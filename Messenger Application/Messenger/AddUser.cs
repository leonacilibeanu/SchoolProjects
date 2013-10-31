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
    public partial class AddUser : Form
    {
        private ListView listView1;
        private String selectedName;

        public AddUser(ListView listView1, String firstNameClicked)
        {
            this.listView1 = listView1;
            InitializeComponent();
            selectedName = firstNameClicked;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Client.groupAddFriend(selectedName, textBox1.Text);
            this.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
