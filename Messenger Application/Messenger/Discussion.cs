using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Media;
using System.Windows;
using System.Threading;
using System.Timers;

namespace Messenger
{
    public partial class Discussion : Form
    {
        int line = 0;
        public String id, name;
        string status = "";

        delegate void SetTextCallback(string text);
        

        public Discussion(String id, String name, String status)
        {
            this.id = id;
            this.name = name;
            this.status = status;
            InitializeComponent();
            this.label3.Text = this.name + " - " + this.status;
            this.richTextBox1.ScrollToCaret();
            this.FormClosed += new FormClosedEventHandler(Form_Close);
            Client.discussions.Add(this);
       
        }

        public void SetText(String text)
        {

            if (this.richTextBox1.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                if(text.Equals("BUZZ!"))
                    button1_Click(null, null);
                else
                    this.richTextBox1.Text = this.richTextBox1.Text + this.name + ":" + text + "\n";
            }
        }

        public void SetFileText(String text)
        {

            if (this.richTextBox1.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetFileText);
                this.Invoke(d, new object[] { text });
            }
            else
                this.richTextBox1.Text += text + "\n";
        }

        private void button3_Click(object sender, EventArgs e)
        {

            if (this.richTextBox1.Text.Equals("BUZZ!"))
            {
                button1_Click(null, null);
            }
            else
            {

                this.richTextBox1.Text = this.richTextBox1.Text + Client.id + ":" + this.richTextBox2.Text + "\n";
                Client.sendMessage(id, richTextBox2.Text);
            }

            this.richTextBox2.Text = "";
            this.richTextBox1.ScrollToCaret();
            line++;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int i=0;
            
            this.richTextBox1.Text = this.richTextBox1.Text + "BUZZ!\n";
            //SoundPlayer simpleSound = new SoundPlayer(@"buzz.wav");
            //simpleSound.Play();

            int x = this.Location.X;
            int y = this.Location.Y;

            for (i = 0; i < 5; i++)
            {
                this.Location = new Point(15+x, y);
                System.Threading.Thread.Sleep(10);
                this.Location = new Point(15+x, 15+y);
                System.Threading.Thread.Sleep(10);
                this.Location = new Point(0+x, 15+y);
                System.Threading.Thread.Sleep(10);
                this.Location = new Point(x,y);
                System.Threading.Thread.Sleep(10);
            }
            this.richTextBox1.ScrollToCaret();

            if (sender != null)
                Client.sendMessage(id, "BUZZ!");
        }

        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == (char)13)
            {
                this.button2.PerformClick();
            }

            //Other if statements if you want to keep an eye out for other keyPresses
        }

        private void Form_Close(object sender, FormClosedEventArgs e)
        {
            Client.discussions.Remove(this);
        }

        private void label3_Click(object sender, EventArgs e)
        {
            
        }

        private void maskedTextBox1_MaskInputRejected(object sender, MaskInputRejectedEventArgs e)
        {

        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        protected void button2_Click(object sender, EventArgs e)
        {
            if(status.Equals("Offline"))
                return;
            Thread newThread = new Thread(ThreadMethod);
            newThread.SetApartmentState(ApartmentState.STA);
            newThread.Start(id);

        }

        static void ThreadMethod(object id)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.ShowDialog();
            Client.sendFile((String)id, dlg.FileName);
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
            richTextBox1.SelectionStart = richTextBox1.Text.Length;
            richTextBox1.ScrollToCaret();
           // richTextBox1.BackColor = Color.White;
            if (richTextBox1.Enabled == false)
                richTextBox1.BackColor = Color.White;
            
        }

        private void richTextBox2_TextChanged(object sender, EventArgs e)
        {

        }

    }
}
