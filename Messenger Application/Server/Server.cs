using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Server
{
    public class Connection
    {
        public String id;
        // Client  socket.
        public Socket sock = null;
        // Size of receive buffer.
        public const int BufferSize = 1024;
        // Receive buffer.
        public byte[] buffer = new byte[BufferSize];
        public byte[] address;
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

            while (data[end] != 0)
                end++;

            pos = end + 1;

            return Encoding.ASCII.GetString(data, aux, end - aux);
        }
    }

    class Server
    {

        public static ManualResetEvent allDone = new ManualResetEvent(false);
        public static List<Connection> clientList = new List<Connection>();
        public static Database db;

        public static byte[] getAddress(Socket s, byte[] port)
        {
            IPEndPoint addr = s.RemoteEndPoint as IPEndPoint;
            byte[] a = new byte[6];

            String[] spl = addr.Address.ToString().Split(new char[] { '.' });

            a[0] = Byte.Parse(spl[0]); a[1] = Byte.Parse(spl[1]);
            a[2] = Byte.Parse(spl[2]); a[3] = Byte.Parse(spl[3]);
            a[4] = port[0]; a[5] = port[1];

            return a;
        }

        public static void sendGroups(String user, Socket sock)
        {
            //get groups
            String groups = db.getGroups(user);
            byte[] data = Encoding.ASCII.GetBytes(groups);
            byte[] len = new byte[2];
            len[0] = (byte)(data.Length >> 8);
            len[1] = (byte)(data.Length & 0xFF);

            //send data
            sock.Send(len);
            sock.Send(data);
        }

        public static void sendFriends(String user, Socket sock)
        {
            //get friends
            String friends = db.getFriends(user);
            byte[] data = Encoding.ASCII.GetBytes(friends);
            byte[] len = new byte[2];
            len[0] = (byte)(data.Length >> 8);
            len[1] = (byte)(data.Length & 0xFF);

            //send data
            sock.Send(len);
            sock.Send(data);
        }

        public static void newConnection(IAsyncResult ar)
        {

            byte[] data = new byte[100];
            char[] buf = new char[100];
            int len;

            allDone.Set();
            // Get the socket that handles the client request.
            Socket listener = (Socket)ar.AsyncState;
            Socket handler = listener.EndAccept(ar);

            // Create the state object.
            Connection state = new Connection();
            state.sock = handler;
            len = handler.Receive(data, 100, 0);

            if (len < 0)
                return;

            String user, pass;

            Parser p = new Parser(data, 1);
            user = p.getNext();
            pass = p.getNext();
            foreach(Connection c in clientList)
                if (c.id.Equals(user))
                {
                    handler.Send(new byte[1] { 0 });
                    handler.Shutdown(SocketShutdown.Both);
                    handler.Close();
                    return;
                }

            //logIn or SignUp
            if ((data[0] == 0 && db.logIn(user, pass)) || (data[0] == 1 && db.registerUser(user, pass)))
            {
                handler.Send(new byte[] { 1 });
                handler.Receive(data, 0, 2, 0);
                state.id = user;
                state.address = getAddress(handler, data);

                //send groups and friends
                sendGroups(user, handler);
                sendFriends(user, handler);

                //send connection information for all friends
                List<String> friends = db.getFriendList(user);
                foreach (Connection c in clientList)
                    if (friends.Contains(c.id))
                        handler.Send(c.address);

                //send ending signal
                handler.Send(new byte[6] { 0, 0, 0, 0, 0, 0 });
                clientList.Add(state);

                //send offline messages
                List<byte[]> messages = db.getOffMessage(state.id);
                foreach (byte[] m in messages)
                {
                    handler.Send(m);
                    handler.Receive(new byte[1]);
                }
                handler.Send(new byte[1] { 0 });

                //start listening for new messages
                handler.BeginReceive(state.buffer, 0, Connection.BufferSize, 0, new AsyncCallback(receiveMessage), state);
            }
            else
            {
                handler.Send(new byte[] { 0 });
                handler.Shutdown(SocketShutdown.Both);
                handler.Close();
            }

        }

        public static void receiveMessage(IAsyncResult ar)
        {
            String content = String.Empty;
            String user = null;
            String user1 = null;
            String message = null;
            bool check;
            byte[] data, aux;

            // Retrieve the state object and the handler socket
            // from the asynchronous state object.
            Connection conn = (Connection)ar.AsyncState;
            Socket handler = conn.sock;

            try
            {
                // Read data from the client socket. 
                int bytesRead = handler.EndReceive(ar);
                if (bytesRead <= 0)
                {
                    clientList.Remove(conn);
                    return;
                }
                Parser p = new Parser(conn.buffer, 1);

                if (bytesRead > 0)
                {

                    switch (conn.buffer[0])
                    {
                        case 2:

                            //get profile
                            user = p.getNext();
                            aux = db.getProfile(user);
                            if (aux == null)
                                aux = new byte[1];
                            handler.Send(aux);

                            break;

                        case 3:

                            //update profile
                            byte[] res = new byte[1] { db.updateProfile(conn.id, conn.buffer) };
                            handler.Send(res);

                            break;

                        case 4:

                            //offline message
                            p.getNext();
                            db.storeOffMessage(p.getNext(), conn.buffer);
                            break;

                        case 5:
                            //friend request
                            user = p.getNext();
                            user1 = p.getNext();
                            check = true;
                            foreach (Connection c in clientList)
                                if (c.id.Equals(user))
                                {
                                    c.sock.Send(conn.buffer);
                                    check = false;
                                    break;
                                }

                            if (check)
                                db.storeOffMessage(user, conn.buffer);
                            break;
                        case 6:
                            //friend request response
                            user = p.getNext();
                            user1 = p.getNext();
                            db.addFriend(user, user1);
                            db.addFriend(user1, user);
                            foreach (Connection c in clientList)
                                if (c.id.Equals(user))
                                {
                                    data = Encoding.ASCII.GetBytes("\0\0\0\0\0\0\0" + user + '\0' + user1);
                                    data[0] = 6;
                                    Buffer.BlockCopy(c.address, 0, data, 1, 6);
                                    c.sock.Send(data);
                                }
                            foreach (Connection c in clientList)
                                if (c.id.Equals(user1))
                                {
                                    data = Encoding.ASCII.GetBytes("\0\0\0\0\0\0\0" + user + '\0' + user1);
                                    data[0] = 6;
                                    Buffer.BlockCopy(c.address, 0, data, 1, 6);
                                    c.sock.Send(data);
                                }
                            break;
                        case 7:
                            //friend request deny
                            user = p.getNext();
                            user1 = p.getNext();
                            foreach (Connection c in clientList)
                                if (c.id.Equals(user))
                                    c.sock.Send(conn.buffer);
                            break;

                        case 8:
                            //add grop
                            db.addGroup(p.getNext(), p.getNext());
                            break;
                        case 9:
                            //add friend to group
                            db.groupAddFriend(p.getNext(), p.getNext(), p.getNext());
                            break;
                        case 10:
                            //delete user from group
                            db.groupDelFriend(p.getNext(), p.getNext());
                            break;
                        case 11:
                            //delete group
                            db.deleteGroup(p.getNext(), p.getNext());
                            break;
                        case 12:
                            //change friend name
                            db.changeNameFriend(p.getNext(), p.getNext(), p.getNext());
                            break;
                        case 13:
                            //change password
                            db.changePassword(p.getNext(), p.getNext());
                            break;

                    }

                    Array.Clear(conn.buffer, 0, Connection.BufferSize);
                    handler.BeginReceive(conn.buffer, 0, Connection.BufferSize, 0, new AsyncCallback(receiveMessage), conn);
                }
                else
                {
                    clientList.Remove(conn);
                }
            }
            catch (Exception e)
            {
                clientList.Remove(conn);
            }
        }

        public static int Main(String[] args)
        {

            IPAddress ipAddress = new IPAddress(new byte[] { 127, 0, 0, 1 });
            IPEndPoint localEndPoint = new IPEndPoint(ipAddress, 11000);
            db = new Database();
            //db.registerUser("adriana", "pass");
            //db.registerUser("andrei", "pass");
            //db.groupAddFriend("alina", "Otto", "amici");
            db.selectFromDB();

            // Create a TCP/IP socket.
            Socket listener = new Socket(AddressFamily.InterNetwork,
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
                Console.WriteLine(e.ToString());
            }

            return 0;
        }
    }
}
