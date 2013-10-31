using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Messenger
{
    public partial class ViewOwnProfile : Form
    {
        public String nume = "", hometown = "", birthday="", about="", id;
        ListView listView1 = new ListView();
        
        
        public ViewOwnProfile(string id)
        {
            InitializeComponent();

            byte[] data = Client.getProfile(id);

            update(data);
            
            bool friends = false;
            foreach (Friend f in Client.friends)
            {
                if (f.id.Equals(id))
                {
                    friends = true;
                    break;
                }
            }

            if (this.id.Equals(Client.id))
                this.button1.Text = "Edit";
            else if (friends)
                this.button1.Text = "Already friends";
            else
                this.button1.Text = "Add";
            //this.button1.Text = "Edit";
            
        }

        public void update(byte[] data)
        {
            Parser p = new Parser(data, 1);

            nume = p.getNext();
            hometown = p.getNext();
            birthday = p.getNext();
            about = p.getNext();
            this.id = p.getNext();
        }

        private void fontDialog1_Apply(object sender, EventArgs e)
        {

        }

        private void Form2_Load(object sender, EventArgs e)
        {
            label5.Text = this.id;
            label7.Text = this.nume;
            label8.Text = this.hometown;
            label9.Text = this.birthday;
            label10.Text = this.about;
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (button1.Text.Equals("Edit"))
            {
                EditProfile editProfile = new EditProfile(this);
                editProfile.Owner = this;

                this.Hide();
                editProfile.Show();
            }
            else
                if (button1.Text.Equals("Add"))
                {
                    Client.addFriend(id);
                }
                else
                    new ChangeFriendName(id).Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Dispose();
            //this.Owner.Dispose();
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }
    }
}
