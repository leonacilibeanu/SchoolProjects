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
    public partial class Login : Form
    {
        public string name, password;
        public Login()
        {
            InitializeComponent();
            this.FormClosed += new FormClosedEventHandler(Form_Close);
            
        }

        private void Form_Close(object sender, EventArgs e)
        {
            System.Environment.Exit(0);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            //bool ok = true;
           // FriendsList friendsList = new FriendsList();
            this.name = textBox1.Text;
            this.password = textBox2.Text;

            Console.WriteLine("{0} {1}", this.name, this.password);

            int res = Client.logIn(this.name, this.password, 0);

           // this.ShowDialog( = DialogResult.OK;

            if (res == 1)
            {
                FriendsList friendsList = new FriendsList();
                friendsList.Owner = this;
                friendsList.Show();

                this.Hide();

                textBox1.Text = "";
                textBox2.Text = "";
            }
            else
            {
                Error error = new Error();
                error.Owner = this;
                error.Show();
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.name = textBox1.Text;
            this.password = textBox2.Text;

            Console.WriteLine("{0} {1}", this.name, this.password);

            int res = Client.logIn(this.name, this.password, 1);

            if (res == 1)
            {
                FriendsList friendsList = new FriendsList();
                friendsList.Owner = this;
                friendsList.Show();

                this.Hide();

                textBox1.Text = "";
                textBox2.Text = "";
            }
            else
            {
                Error error = new Error();
                error.Owner = this;
                error.Show();
            }
        }

        private void Login_Load(object sender, EventArgs e)
        {

        }
    }
}
