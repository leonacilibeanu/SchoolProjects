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
    public partial class FindFriend : Form
    {
        ListView listView1;

        public FindFriend(ListView listView1)
        {
            InitializeComponent();
            this.listView1 = listView1;
        }

        private void AddFriend_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            ViewOwnProfile profile = new ViewOwnProfile(textBox1.Text);
            profile.Owner = this;
            profile.Show();

            this.Hide();
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
