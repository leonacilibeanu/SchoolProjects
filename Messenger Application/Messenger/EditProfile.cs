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
    public partial class EditProfile : Form
    {

        ViewOwnProfile parent;
        public EditProfile(ViewOwnProfile p)
        {
            InitializeComponent();
            parent = p;
            textBox1.Text = p.nume;
            label5.Text = p.id;
            textBox2.Text = p.hometown;
            textBox3.Text = p.birthday;
            textBox4.Text = p.about;

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void Form4_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] profile = Encoding.ASCII.GetBytes('\0' + textBox1.Text + '\0' + textBox2.Text + '\0'
                                + textBox3.Text + '\0' + textBox4.Text + '\0' + Client.id);
            Console.WriteLine(Encoding.ASCII.GetString(profile));
            Client.updateProfile(profile);
            this.Hide();
            parent.update(profile);
            this.Owner.Dispose();
            new ViewOwnProfile(Client.id).Show();
            this.Dispose();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Password changePassword = new Password();
            changePassword.Owner = this;
            changePassword.Show();
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
