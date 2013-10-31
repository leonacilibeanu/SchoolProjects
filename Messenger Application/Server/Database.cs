using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;


/* Asta trebuie facut cand (probabil) pornim server-ul ca sa initalizam baza de date 
 * 
 * Data_connection db_conn = new Data_connection();
 * db_conn.DatabaseConnection();
 * 
 **/


namespace Server
{
    class Database
    {
        public String connectionString = null;
        public SqlConnection dbConnection;

        /* Functions used for database communication **/

        /*
         * Database connection
         **/
        public Database()
        {
            connectionString = "Data Source=.\\SQLEXPRESS;AttachDbFilename=C:\\Users\\Andrei\\Desktop\\Server\\Datastore.mdf;Integrated Security=True;User Instance=True; MultipleActiveResultSets=true";
            dbConnection = new SqlConnection(connectionString);
            dbConnection.Open();

        }

        /*
        * Commit to database
        * param: database operation (select, insert, update, delete)
        **/
        private int commitDB(SqlCommand cmd)
        {
            int rows = 0;

            try
            {
                //dbConnection.Open();
                rows += cmd.ExecuteNonQuery();
                //dbConnection.Close();
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return rows;
        }

        /* Table User_data **/

        /*
         * Check if an user identified with userId and password exist in database
         * param: userId
         * param: password
         * return: true if user exist, otherwise false
         **/
        public Boolean logIn(String userId, String password)
        {
            Boolean find_user = true;

            try
            {
                //dbConnection.Open();

                // select profile and store or check if exist
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandText = "SELECT Profile FROM User_data WHERE UserId=@UserId AND Password=@Password";
                cmd.Parameters.AddWithValue("@UserId", userId);
                cmd.Parameters.AddWithValue("@Password", password);

                SqlDataReader reader = cmd.ExecuteReader();

                find_user &= reader.HasRows;

                reader.Close();

                //dbConnection.Close();
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return find_user;
        }

        /*
         * Check if an user identified with userId and password exist in database
         * param: userId
         * param: password
         * return: true if user exist, otherwise false
         **/
        public bool signUp(String userId, String password)
        {
            return logIn(userId, password);
        }

        /*
         * Check if an user with userId exist in database
         * param: userId
         * return: true if user exist, otherwise false
         * **/
        public Boolean existUser(String userId)
        {
            Boolean exist = true;

            try
            {
                //dbConnection.Open();

                // select profile and local store or check if exist
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandText = "SELECT Profile FROM User_data WHERE UserId=@UserId";
                cmd.Parameters.AddWithValue("@UserId", userId);

                SqlDataReader reader = cmd.ExecuteReader();

                exist &= reader.HasRows;

                reader.Close();

                //dbConnection.Close();
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return exist;
        }

        /*
         * Register an user with userId, password and null profile
         * param: userId
         * param: password
         * return: true if user is save in database with success, false if user exist
         **/
        public Boolean registerUser(String userId, String password)
        {
            if (logIn(userId, password))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "INSERT INTO User_data (UserId, Password, Profile) VALUES (@UserId, @Password, @Profile)";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@Password", password);
            cmd.Parameters.AddWithValue("@Profile", Encoding.ASCII.GetBytes("\0\0\0\0\0" + userId));
            addGroup(userId, "Friends");

            commitDB(cmd);
            return true;
        }

        /*
         * Change password for an user with userId
         * param: userId
         * param: newPassword that is used for update password field in database
         * return: true if update finalize with success, false if user don't exist
         **/
        public Boolean changePassword(String userId, String newPassword)
        {
            if (!existUser(userId))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "UPDATE User_data SET Password=@NewPassword WHERE UserId=@UserId";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@NewPassword", newPassword);
            commitDB(cmd);

            return true;
        }

        /*
         * Change profile for an user with userId
         * param: userId
         * param: newProfile that is used for update Profile field in database
         * return: true if update finalize with success, false if user don't exist
         **/
        public byte updateProfile(String userId, byte[] newProfile)
        {
            if (!existUser(userId))
            {
                return 0;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "UPDATE User_data SET Profile=@NewProfile WHERE UserId=@UserId";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@NewProfile", newProfile);
            commitDB(cmd);

            return 1;
        }

        /*
         * Get profile for an user with userId
         * param: userId
         * return: user's profile (byte[]) if user exist, null if user don't exist or profile is not completed
         **/
        public byte[] getProfile(String userId)
        {
            if (!existUser(userId))
            {
                return null;
            }

            byte[] profile = null;
            try
            {
                //dbConnection.Open();

                // select
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandType = CommandType.Text;
                cmd.CommandText = "SELECT Profile FROM User_data WHERE UserId=@UserId";
                cmd.Parameters.AddWithValue("@UserId", userId);

                SqlDataReader reader = cmd.ExecuteReader();

                if (reader.HasRows && reader.Read())
                {
                    if (DBNull.Value.Equals(reader["Profile"]))
                    {
                        profile = null;
                    }
                    else
                    {
                        profile = (byte[])reader["Profile"];
                    }
                    reader.Close();
                    //dbConnection.Close();
                }
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return profile;
        }

        /* Table Message_data **/

        /*
         * Store an offline message in database
         * param: userId = user who receive the message 
         * param: sender = user who send the message
         * param: message
         * return: true if message is saved in database with success, false if userId or sender don't exist
         **/
        public Boolean storeOffMessage(String userId, byte[] message)
        {
            if (!existUser(userId))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "INSERT INTO Message_data (UserId, Message_off) VALUES (@UserId, @Message_off)";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@Message_off", message);

            commitDB(cmd);
            return true;
        }

        /*
         * Get offline messages for an user with userId
         * param: userId
         * return: a pair<sender, off_message> list, null if userId don't exist
         **/
        public List<byte[]> getOffMessage(String userId)
        {
            if (!existUser(userId))
            {
                return null;
            }

            List<byte[]> messages = new List<byte[]>();

            try
            {
                //dbConnection.Open();

                // select
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandType = CommandType.Text;
                cmd.CommandText = "SELECT UserId, Message_off FROM Message_data WHERE UserId=@UserId";
                cmd.Parameters.AddWithValue("@UserId", userId);

                SqlDataReader reader = cmd.ExecuteReader();

                if (reader.HasRows)
                {
                    while (reader.Read())
                    {
                        byte[] msg_byte = (byte[])reader["Message_off"];
                        //String sender = (String)reader["SenderId"];
                        messages.Add(msg_byte);
                    }

                    reader.Close();
                    //dbConnection.Close();
                }
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            deleteOffMessage(userId);
            return messages;
        }

        public Boolean deleteOffMessage(String userId)
        {
            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "DELETE FROM Message_data WHERE UserId=@UserId";
            cmd.Parameters.AddWithValue("@UserId", userId);

            commitDB(cmd);
            return true;
        }


        /* Table Friend_data **/

        public Boolean existFriend(String userId, String friendId)
        {
            Boolean find_friend = true;
            try
            {
                //dbConnection.Open();

                // select profile and store or check if exist
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandText = "SELECT UserId FROM Friend_data WHERE UserId=@UserId AND FriendId=@FriendId";
                cmd.Parameters.AddWithValue("@UserId", userId);
                cmd.Parameters.AddWithValue("@FriendId", friendId);

                SqlDataReader reader = cmd.ExecuteReader();

                find_friend &= reader.HasRows;

                reader.Close();

                //dbConnection.Close();
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return find_friend;
        }

        /*
         * Add a friend for a valid userId; friend have FriendName=FriendId and GroupId="Friends"
         * param: userId
         * param: friendId
         * return: true is friend is saved with success, false if userId or friendId don't exist
         **/
        public Boolean addFriend(String userId, String friendId)
        {
            if (!existUser(userId) || !existUser(friendId))
            {
                return false;
            }

            if (existFriend(userId, friendId))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "INSERT INTO Friend_data (UserId, FriendId, FriendName, GroupId) VALUES (@UserId, @FriendId, @FriendName, @GroupId)";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@FriendId", friendId);
            cmd.Parameters.AddWithValue("@FriendName", friendId);
            cmd.Parameters.AddWithValue("@GroupId", "Friends");

            commitDB(cmd);
            return true;
        }

        /*
         * Add a friend to a group
         * param: userId
         * param: friendId
         * param: newGroup
         * return: true if group field is updated with success, false if userId or friendId don't exist
         **/
        public Boolean groupAddFriend(String userId, String friendId, String newGroup)
        {
            if (!existUser(userId) || !existUser(friendId))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "UPDATE Friend_data SET GroupId=@NewGroup WHERE UserId=@UserId and FriendId=@FriendId";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@FriendId", friendId);
            cmd.Parameters.AddWithValue("@NewGroup", newGroup);

            commitDB(cmd);
            return true;
        }

        /*
         * Delete a group for a user => change friend's group with "Friends" group
         * param: userId
         * param: friendId
         * param: groupId
         * return: true if group field is updated with success, false if userId or friendId don't exist
         **/
        public Boolean groupDelFriend(String userId, String friendId)
        {
            return groupAddFriend(userId, friendId, "Friends");
        }

        /*
         * Change group for all user that have this group with "Friends"; function used
         * when delete a group
         * param: userId
         * param: oldGroup
         * return: true if group field is updated with success, false if userId don't exist
         **/
        public Boolean changeGroupAll(String userId, String oldGroup)
        {
            if (!existUser(userId))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "UPDATE Friend_data SET GroupId=@NewGroup WHERE UserId=@UserId and GroupId=@OldGroup";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@OldGroup", oldGroup);
            cmd.Parameters.AddWithValue("@NewGroup", "Friends");

            commitDB(cmd);
            return true;
        }

        /*
         * Change name for a valid friend of a valid userId
         * param: userId
         * param: friendId
         * param: newName
         * return: true if name field is updated with success, false if userId or friendId don't exist
         **/
        public Boolean changeNameFriend(String userId, String friendId, String newName)
        {
            if (!existUser(userId) || !existUser(friendId))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "UPDATE Friend_data SET FriendName=@NewName WHERE UserId=@UserId and FriendId=@FriendId";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@FriendId", friendId);
            cmd.Parameters.AddWithValue("@NewName", newName);

            commitDB(cmd);
            return true;
        }

        /*
         * Get friend for a valid user
         * param: userId
         * return: friendId, groupIf, friendName for each friend
         **/
        public String getFriends(String userId)
        {
            /*if (!existUser(userId))
            {
                return null;
            }*/

            String friends = "";

            try
            {
                //dbConnection.Open();

                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandType = CommandType.Text;
                cmd.CommandText = "SELECT FriendId, GroupId, FriendName FROM Friend_data WHERE UserId=@UserId";
                cmd.Parameters.AddWithValue("@UserId", userId);

                SqlDataReader reader = cmd.ExecuteReader();

                if (reader.HasRows)
                {
                    reader.Read();
                    String friend = (String)reader["FriendId"];
                    String group = (String)reader["GroupId"];
                    String name = (String)reader["FriendName"];
                    friends = friends + friend + '\0' + group + '\0' + name;

                    while (reader.Read())
                    {
                        friend = (String)reader["FriendId"];
                        group = (String)reader["GroupId"];
                        name = (String)reader["FriendName"];
                        friends = friends + '\0' + friend + '\0' + group + '\0' + name;
                    }

                    reader.Close();
                    //dbConnection.Close();
                }
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return friends;
        }

        public List<String> getFriendList(String userId)
        {
            if (!existUser(userId))
            {
                return null;
            }

            List<String> friends = new List<String>();

            try
            {
                //dbConnection.Open();

                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandType = CommandType.Text;
                cmd.CommandText = "SELECT FriendId FROM Friend_data WHERE UserId=@UserId";
                cmd.Parameters.AddWithValue("@UserId", userId);

                SqlDataReader reader = cmd.ExecuteReader();
                String friend;

                if (reader.HasRows)
                {
                    while (reader.Read())
                    {
                        friend = (String)reader["FriendId"];
                        friends.Add(friend);
                    }

                    reader.Close();
                    //dbConnection.Close();
                }
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return friends;
        }

        /* Table Group_data **/

        public Boolean existGroup(String userId, String groupId)
        {
            Boolean find_group = true;
            try
            {
                //dbConnection.Open();

                // select profile and store or check if exist
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandText = "SELECT UserId FROM Group_data WHERE UserId=@UserId AND GroupId=@GroupId";
                cmd.Parameters.AddWithValue("@UserId", userId);
                cmd.Parameters.AddWithValue("@GroupId", groupId);

                SqlDataReader reader = cmd.ExecuteReader();

                find_group &= reader.HasRows;

                reader.Close();

                //dbConnection.Close();
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return find_group;
        }

        /*
         * Store a new group for a valid userId
         * param: userId 
         * param: group
         * return: true if group is saved in database with success, false if userId don't exist, or group already exist
         **/
        public Boolean addGroup(String userId, String group)
        {
            if (!existUser(userId))
            {
                return false;
            }

            if (existGroup(userId, group))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "INSERT INTO Group_data (UserId, GroupId) VALUES (@UserId, @GroupId)";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@GroupId", group);

            commitDB(cmd);
            return true;
        }

        /*
         * Delete a group for a valid userId
         * param: userId
         * param: group name
         * return: true if group is deleted with success, false if userId don't exist
         **/
        public Boolean deleteGroup(String userId, String group)
        {
            if (!existUser(userId))
            {
                return false;
            }

            SqlCommand cmd = new SqlCommand();
            cmd.CommandType = CommandType.Text;
            cmd.Connection = dbConnection;
            cmd.CommandText = "DELETE FROM Group_data WHERE GroupId=@GroupId AND UserId=@UserId";
            cmd.Parameters.AddWithValue("@UserId", userId);
            cmd.Parameters.AddWithValue("@GroupId", group);

            commitDB(cmd);

            return changeGroupAll(userId, group);
        }

        /*
         * Get list of groups for a valid userId
         * param: userId
         * return: list of groups, null if userId don't exist
         **/
        public String getGroups(String userId)
        {
            /*if (!existUser(userId))
            {
                return null;
            }*/

            String groups = "";

            try
            {
                //dbConnection.Open();

                // select
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandType = CommandType.Text;
                cmd.CommandText = "SELECT GroupId FROM Group_data WHERE UserId=@UserId";
                cmd.Parameters.AddWithValue("@UserId", userId);

                SqlDataReader reader = cmd.ExecuteReader();

                if (reader.HasRows)
                {
                    reader.Read();
                    String group = (String)reader["GroupId"];
                    groups = groups + group;

                    while (reader.Read())
                    {
                        group = (String)reader["GroupId"];
                        groups = groups + '\0' + group;
                    }

                    reader.Close();
                    //dbConnection.Close();
                }
            }
            catch (SqlException e)
            {
                Console.WriteLine(e);
            }

            return groups;
        }


        /* Test **/

        public void selectFromDB()
        {
            try
            {
                //dbConnection.Open();
                // select
                SqlCommand cmd = dbConnection.CreateCommand();
                cmd.CommandText = "SELECT * FROM User_data";

                SqlDataReader reader = cmd.ExecuteReader();

                if (reader.HasRows)
                {
                    while (reader.Read())
                    {
                        Console.WriteLine("UserId = {0}", reader["UserId"]);
                        //Console.WriteLine("FriendId = {0}", reader["FriendId"]);
                        //Console.WriteLine("GroupId = {0}", reader["GroupId"]);
                        //Console.WriteLine("FriendName = {0}", reader["FriendName"]);
                    }
                }

                reader.Close();
                //dbConnection.Close();
            }
            catch (SqlException e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
