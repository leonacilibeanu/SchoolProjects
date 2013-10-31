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
    public partial class CreateAccount : Form
    {
        string nume, pass1, pass2;
        public CreateAccount()
        {
            InitializeComponent();

        }

        private void button1_Click(object sender, EventArgs e)
        {
            pass1 = this.textBox2.Text;
            pass2 = this.textBox3.Text;
            nume = this.textBox1.Text;
            if(pass1 == pass2) 
            {
                //if (Client.logIn(, pass1) == 1)
                MessageBox.Show("Thank you " + nume + "\n Your account was created!", "Welcome to the club!");
               // this.Close();

                this.Owner.Show();
                this.Close();
            }
            else 
            {
                nume = "Password don't match!";
                MessageBox.Show(nume,"Welcome !");
            }           // Console.WriteLine();


        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
           
            // Console.WriteLine("bla bla {0}\n",s);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox3_TextChanged(object sender, EventArgs e)
        {

        }
            
    }
}
