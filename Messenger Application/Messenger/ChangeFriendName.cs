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
    public partial class ChangeFriendName : Form
    {
        String id;

        public ChangeFriendName(String id)
        {
            InitializeComponent();
            this.id = id;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Client.changeFriendName(id, textBox1.Text);
            this.Close();
        }
    }
}
