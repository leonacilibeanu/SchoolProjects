using System;
using System.Net;
using System.Linq;
using System.Net.Sockets;
using System.Threading;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.Windows.Forms;
using System.IO;

namespace Messenger
{
    // State object for receiving data from remote device.
    public class Connection
    {
        public String id, fileName;
        public Socket sock = null;
        public const int BufferSize = 256;
        public byte[] buffer = new byte[BufferSize];
        public byte[] address = new byte[6];
        public Friend f;
        public FileStream file;
    }

    public class Parser
    {
        byte[] data;
        int pos;

        public Parser(byte[] data, int pos)
        {
            this.data = data;
            this.pos = pos;
        }

        public String getNext()
        {
            int end = pos, aux = pos;

            if (end >= data.Length)
                return "";

            while (end < data.Length && data[end] != 0)
                end++;

            pos = end + 1;

            return Encoding.ASCII.GetString(data, aux, end - aux);
        }
    }
    
    public class Friend
    {
        public String id, name, group, status;
        public Connection conn;

        public Friend(String id, String group, String name)
        {
            this.id = id;
            this.group = group;
            this.name = name;
            this.status = "Offline";
        }

        public override String ToString()
        {
            return name + " " + group + " " + id;
        }
    }

    public class Client
    {
        //client data
        public static int sPort = 11000, myPort;
        public static byte[] myAddr, serverAddr;
        public static Socket serverSock, listener;
        public static IPAddress ipAddress;
        public static String id, status;
        public static ManualResetEvent allDone = new ManualResetEvent(false);
        public static List<Connection> connections = new List<Connection>();
        public static List<String> groups = new List<String>();
        public static List<Friend> friends = new List<Friend>();
        public static List<Discussion> discussions = new List<Discussion>();

        public static void getGroups(Socket sock)
        {
            byte[] len = new byte[2];
            sock.Receive(len, 2, 0);
            int length = (len[0] << 8) + len[1];
            int offset = 0, received;

            //get data
            byte[] data = new byte[length];
            while (length > 0)
            {
                received = sock.Receive(data, offset, length, 0);
                offset += received;
                length -= received;
            }

            Parser p = new Parser(data, 0);
            String group = p.getNext();

            while (!group.Equals(""))
            {
                groups.Add(group);
                group = p.getNext();
            }
        }

        public static void getFriends(Socket sock)
        {
            byte[] len = new byte[2];
            sock.Receive(len, 2, 0);
            int length = (len[0] << 8) + len[1];
            int offset = 0, received;

            //get data
            byte[] data = new byte[length];
            while (length > 0)
            {
                received = sock.Receive(data, offset, length, 0);
                offset += received;
                length -= received;
            }

            Parser p = new Parser(data, 0);
            String id = p.getNext(), group, name;

            while (!id.Equals(""))
            {
                group = p.getNext();
                name = p.getNext();
                friends.Add(new Friend(id, group, name));
                id = p.getNext();
            }
        }

        public static int logIn(String name, String pass, int mode)
        {

            byte[] data = Encoding.ASCII.GetBytes('\0' + name + "\0" + pass);
            byte[] addr = new byte[4];
            int port;
            Connection conn;

            // Connect to a remote device.
            try
            {
                //connect to server
                IPAddress ipAddress = new IPAddress(serverAddr);

                serverSock = new Socket(AddressFamily.InterNetwork,
                    SocketType.Stream, ProtocolType.Tcp);

                serverSock.Connect(ipAddress, sPort);

                //send login/signup information to server
                data[0] = (byte)mode;
                serverSock.Send(data);

                //receive response
                byte[] recv = new byte[8192];
                serverSock.Receive(recv, 0, 1, 0);

                if (recv[0] == 0)
                {
                    //if login is invalid close connection
                    serverSock.Shutdown(SocketShutdown.Both);
                    serverSock.Close();
                    return 0;
                }

                //send listening port to server
                id = name;
                status = "Available";
                recv[0] = (byte)(myPort >> 8);
                recv[1] = (byte)(myPort & 0xFF);
                serverSock.Send(recv, 0, 2, 0);

                //get groups and friends
                getGroups(serverSock);
                getFriends(serverSock);

                //get connection info for friends
                serverSock.Receive(recv, 0, 6, 0);
                while (recv[0] != 0 || recv[1] != 0 || recv[2] != 0 || recv[3] != 0)
                {
                    connect(recv, 0);

                    //get next connection info
                    serverSock.Receive(recv, 0, 6, 0);

                }

                //get offline messages
                serverSock.Receive(recv);
                while (recv[0] == 4)
                {
                    serverSock.Send(new byte[1]);
                    Parser p = new Parser(recv, 1);
                    String from = p.getNext();
                    p.getNext();
                    String message = p.getNext();
                    displayMessage(from, message);
                    serverSock.Receive(recv);
                }

                return 1;
            }
            catch (Exception e)
            {
                return 0;
            }
        }

        public static void connect(byte[] recv, int o)
        {
            //connect to friend
            byte[] addr = new byte[4];
            int port;
            Connection conn;

            addr[0] = recv[0 + o]; addr[1 + o] = recv[1 + o];
            addr[2] = recv[2 + o]; addr[3] = recv[3 + o];
            port = (recv[4] << 8) + recv[5];
            conn = new Connection();
            conn.sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            conn.sock.Connect(new IPAddress(addr), port);
            Buffer.BlockCopy(recv, 0, conn.address, 0, 6);

            //send id
            conn.sock.Send(new byte[] { 0 });
            conn.sock.Send(Encoding.ASCII.GetBytes(id));

            //receive id
            Array.Clear(recv, 0, 20);
            conn.sock.Receive(recv);
            Parser p = new Parser(recv, 0);
            conn.id = p.getNext();
            conn.f = null;

            //add friend and start listening
            connections.Add(conn);
            foreach (Friend f in friends)
                if (f.id.Equals(conn.id))
                {
                    f.conn = conn;
                    conn.f = f;
                    f.status = p.getNext();
                    break;
                }
            if (conn.f == null)
            {
                conn.f = new Friend(conn.id, "Friends", conn.id);
                conn.f.conn = conn;
                friends.Add(conn.f);
            }

            conn.sock.Send(myAddr);
            conn.sock.BeginReceive(conn.buffer, 0, Connection.BufferSize, 0, new AsyncCallback(receiveMessage), conn);
        }

        public static void newConnection(IAsyncResult ar)
        {
            Socket listener = (Socket)ar.AsyncState;
            Socket handler = listener.EndAccept(ar);
            byte[] data = new byte[100];
            Connection conn = new Connection();

            handler.Receive(data, 1, 0);
            if (data[0] == 1)
            {
                //file transfer
                conn.sock = handler;
                handler.Receive(data);
                Parser p = new Parser(data, 0);
                conn.id = p.getNext();
                conn.fileName = p.getNext();
                displayFileMessage(conn.id, "Started receiving " + conn.fileName);
                conn.sock.Send(new byte[] { 0 });

                //open file and begin transfer
                conn.file = new FileStream(conn.fileName, FileMode.Create);
                conn.sock.BeginReceive(conn.buffer, 0, Connection.BufferSize, 0, new AsyncCallback(receiveFile), conn);
                return;
            }

            //get id of newly connected friend
            handler.Receive(data);

            conn.sock = handler;
            conn.id = Encoding.ASCII.GetString(data).TrimEnd('\0');
            conn.sock.Send(Encoding.ASCII.GetBytes(id + '\0' + status));

            //get address
            handler.Receive(conn.address, 0, 6, 0);

            foreach(Friend f in friends)
                if (f.id.Equals(conn.id))
                {
                    f.conn = conn;
                    conn.f = f;
                    f.status = "Available";
                    break;
                }

            if (conn.f == null)
            {
                conn.f = new Friend(conn.id, "Friends", conn.id);
                conn.f.conn = conn;
                friends.Add(conn.f);
            }

            //send my id to new connection
            connections.Add(conn);

            //start listening for new data
            conn.sock.BeginReceive(conn.buffer, 0, Connection.BufferSize, 0, new AsyncCallback(receiveMessage), conn);
            allDone.Set();
        }

        public static byte[] getProfile(String user)
        {
            byte[] recv = new byte[1024], send;

            send = Encoding.ASCII.GetBytes("\0" + user);
            
            send[0] = 2;
            serverSock.Send(send);
            serverSock.Receive(recv);
            return recv;
        }

        public static byte updateProfile(byte[] profile)
        {
            byte[] recv = new byte[1];

            profile[0] = 3;
            serverSock.Send(profile);
            serverSock.Receive(recv, 0, 1, 0);

            return recv[0];
        }

        public static void displayMessage(String from, String message)
        {
            bool check = false;

            Friend fr = null;

            foreach (Friend f in friends)
                if (f.id.Equals(from))
                {
                    fr = f;
                    break;
                }

            foreach (Discussion d in discussions)
                if (d.id.Equals(from))
                {
                    d.SetText(message);
                    check = true;
                }
            if (!check)
            {
                Discussion d = new Discussion(from, fr.name, fr.status);
                new Thread(threadDisc).Start(d);
                d.SetText(message);
            }

        }

        public static void displayFileMessage(String from, String message)
        {
            bool check = false;

            Friend fr = null;

            foreach (Friend f in friends)
                if (f.id.Equals(from))
                {
                    fr = f;
                    break;
                }

            foreach (Discussion d in discussions)
                if (d.id.Equals(from))
                {
                    d.SetFileText(message);
                    check = true;
                }
            if (!check)
            {
                Discussion d = new Discussion(from, fr.name, fr.status);
                new Thread(threadDisc).Start(d);
                d.SetFileText(message);
            }

        }

        public static void receiveMessage(IAsyncResult ar)
        {
            String content = String.Empty;
            String file, user, user1, from, to, message;

            // Retrieve the state object and the handler socket
            // from the asynchronous state object.
            Connection conn = (Connection)ar.AsyncState;
            Socket handler = conn.sock;

            try
            {

                // Read data from the client socket. 
                int bytesRead = handler.EndReceive(ar);
                Parser p = new Parser(conn.buffer, 1);

                if (bytesRead > 0)
                {

                    switch (conn.buffer[0])
                    {
                        case 0:

                            //message
                            from = p.getNext();
                            to = p.getNext();
                            message = p.getNext();

                            displayMessage(from, message);          

                            break;

                        case 1:
                            //file
                            file = p.getNext();
                            break;

                        case 3:
                            //status update
                            from = p.getNext();
                            foreach(Friend f in friends)
                                if (f.id.Equals(from))
                                {
                                    f.status = p.getNext();
                                    break;
                                }
                            break;

                        case 4:
                            //friend request
                            user = p.getNext();
                            break;
                        case 5:
                            user = p.getNext();
                            user1 = p.getNext();
                            break;
                        case 6:
                            p = new Parser(conn.buffer, 7);
                            user = p.getNext();
                            user1 = p.getNext();
                            if (user.Equals(id) && conn.buffer[1] != 0 && conn.buffer[2] != 0 && conn.buffer[3] != 0
                                    && conn.buffer[4] != 0)
                            {
                                connect(conn.buffer, 1);
                            }
                            break;
                        case 7:
                            user = p.getNext();
                            break;
                    }

                    Array.Clear(conn.buffer, 0, Connection.BufferSize);
                    handler.BeginReceive(conn.buffer, 0, Connection.BufferSize, 0, new AsyncCallback(receiveMessage), conn);
                }
                else
                {
                    conn.f.status = "Offline";
                    connections.Remove(conn);
                }
            }
            catch (Exception) 
            { 
                conn.f.status = "Offline";
                connections.Remove(conn);
            }
        }

        public static void sendMessage(String user, String message)
        {
            byte[] msg;

            msg = Encoding.ASCII.GetBytes('\0' + id + '\0' + user + '\0' + message);
            msg[0] = 0;

            //send to correct user
            foreach(Connection c in connections)
                if (c.id.Equals(user))
                {
                    c.sock.Send(msg);
                    return;
                }

            //send offline message to server
            msg[0] = 4;
            serverSock.Send(msg);
        }

        public static void sendFile(String user, String file)
        {
            byte[] addr = new byte[4];
            int port = 0;
            Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            foreach (Connection c in connections)
                if (c.id.Equals(user))
                {
                    Buffer.BlockCopy(c.address, 0, addr, 0, 4);
                    port = (c.address[4] << 8) + c.address[5];
                    break;
                }

            //make new connection to friend
            sock.Connect(new IPAddress(addr), port);
            sock.Send(new byte[] { 1 });
            
            //send transfer information
            String[] aux = file.Split('\\');
            String fileName = aux[aux.Length - 1]; 
            sock.Send(Encoding.ASCII.GetBytes(id + '\0' + fileName));
            sock.Receive(addr);

            displayFileMessage(user, "Started sending " + fileName);

            //save information
            Connection conn = new Connection();
            conn.id = user;
            conn.fileName = fileName;
            conn.sock = sock;
            sock.BeginSendFile(file, new AsyncCallback(sendFileCallback), conn);
            listener.BeginAccept(new AsyncCallback(newConnection), listener);

        }

        public static void sendFileCallback(IAsyncResult ar)
        {
            Connection conn = (Connection)ar.AsyncState;

            //display message
            conn.sock.EndSendFile(ar);
            displayFileMessage(conn.id, "Finished sending " + conn.fileName);
            conn.sock.Shutdown(SocketShutdown.Both);
            conn.sock.Close();
        }

        public static void receiveFile(IAsyncResult ar)
        {
            Connection conn = (Connection)ar.AsyncState;
            byte[] data = new byte[8192];

            try
            {
                int received = conn.sock.EndReceive(ar);

                if (received > 0)
                {

                    conn.file.Write(conn.buffer, 0, received);
                    conn.sock.BeginReceive(conn.buffer, 0, Connection.BufferSize, 0, new AsyncCallback(receiveFile), conn);
                }
                else
                {
                    if (received == 0)
                    {
                        displayFileMessage(conn.id, "Finished receiving " + conn.fileName);
                    }
                    else
                    {
                        displayFileMessage(conn.id, "Error receiving " + conn.fileName);
                    }

                    conn.file.Close();
                    conn.sock.Shutdown(SocketShutdown.Both);
                    conn.sock.Close();
                }
            }
            catch (Exception)
            {
                displayFileMessage(conn.id, "Error receiving " + conn.fileName);
                conn.sock.Shutdown(SocketShutdown.Both);
                conn.sock.Close();
            }
        }

        public static void changeStatus(String status)
        {
            byte[] data = Encoding.ASCII.GetBytes('\0' + Client.id + '\0' + status);
            data[0] = 3;
            foreach (Connection c in connections)
                c.sock.Send(data);
        }

        public static void addFriend(String user)
        {
            byte[] data = Encoding.ASCII.GetBytes("\0" + user + "\0" + id);
            data[0] = 6;
            serverSock.Send(data);
        }

        public static void addGroup(String group)
        {
            byte[] data = Encoding.ASCII.GetBytes('\0' + id + '\0' + group);
            data[0] = 8;
            serverSock.Send(data);
        }

        public static void groupAddFriend(String friend, String group)
        {
            byte[] data = Encoding.ASCII.GetBytes('\0' + id + '\0' + friend + '\0' + group);
            data[0] = 9;
            serverSock.Send(data);
        }

        public static void groupDelFriend(String friend)
        {
            byte[] data = Encoding.ASCII.GetBytes('\0' + id + '\0' + friend);
            data[0] = 10;
            serverSock.Send(data);
        }

        public static void deleteGroup(String group)
        {
            byte[] data = Encoding.ASCII.GetBytes('\0' + id + '\0' + group);
            data[0] = 11;
            serverSock.Send(data);
        }

        public static void changeFriendName(String friend, String newName)
        {
            byte[] data = Encoding.ASCII.GetBytes('\0' + id + '\0' + friend + '\0' + newName);
            data[0] = 12;
            serverSock.Send(data);
        }

        public static void changePassword(String pass)
        {
            byte[] data = Encoding.ASCII.GetBytes('\0' + id + '\0' + pass);
            data[0] = 13;
            serverSock.Send(data);
        }

        public static void logOut()
        {
            foreach (Connection c in connections)
            {
               try
                {
                    c.sock.Shutdown(SocketShutdown.Both);
                    c.sock.Close();
                }
                catch (Exception) { }
            }

            try
            {
                serverSock.Shutdown(SocketShutdown.Both);
                serverSock.Close();
            }
            catch (Exception) { }

            groups.Clear();
            connections.Clear();
            friends.Clear();
            discussions.Clear();
        }


        public static void threadFunc()
        {
            Application.EnableVisualStyles();
            Application.Run(new Login());
            
        }

        public static void threadDisc(object d)
        {
            Application.EnableVisualStyles();
            Application.Run((Discussion)d);
        }

        public static int Main(String[] args)
        {

            //start UI
            Thread t = new Thread(new ThreadStart(threadFunc));
            t.Start();
            
            //get address
            myAddr = new byte[6];
            serverAddr = new byte[4] { 127, 0, 0, 1 };
            Buffer.BlockCopy(new byte[4] {127, 0, 0, 1}, 0, myAddr, 0, 4);
            myPort = Int32.Parse(args[0]);
            //myPort = 15000;
            myAddr[4] = (byte)(myPort >> 8);
            myAddr[5] = (byte)(myPort & 0xFF);
            
            ipAddress = new IPAddress(new byte[] { 127, 0, 0, 1 });
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, myPort);

            // Create a TCP/IP socket.
            listener = new Socket(AddressFamily.InterNetwork,
                SocketType.Stream, ProtocolType.Tcp);

            // Bind the socket to the local endpoint and listen for incoming connections.
            try
            {
                listener.Bind(localEndPoint);
                listener.Listen(100);

                while (true)
                {
                    allDone.Reset();

                    // Start an asynchronous socket to listen for connections.
                    listener.BeginAccept(new AsyncCallback(newConnection), listener);

                    allDone.WaitOne();
                }

            }
            catch (Exception e)
            {
            }
            
            return 0;
        }
    }
}