using System;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace Messenger
{
    public partial class FriendsList : Form
    {
        private int i = 0;
        private int numberOfClicks = 0;
        private string firstNameClicked;
        private int firstIndexClicked = -1;
        private bool itemSingleClicked = false;
        private bool itemDoubleClicked = false;
        static List<threadparams> thrparams = new List<threadparams>();

        public FriendsList()
        {
            InitializeComponent();
            InitializeListView();
            InitializeMenuStrip();
            this.FormClosed += new FormClosedEventHandler(Form_Close);
        }

        protected override void WndProc(ref Message m)
        {
            bool passMessageOn = true;
            bool selected = (listView1.SelectedItems.Count == 1);
            string selectedGroupItem;
            //Console.WriteLine(m.Msg);

            if (m.Msg == 0x0210)
            {
                if ((itemDoubleClicked == false) && (itemSingleClicked == false) && (selected))
                {
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[1].ForeColor = Color.Gray;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[5].ForeColor = Color.Black;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[6].ForeColor = Color.Gray;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[7].ForeColor = Color.Gray;

                    selectedGroupItem = listView1.SelectedItems[0].Group.Header;
                    Console.WriteLine(selectedGroupItem);
                }

                if ((itemDoubleClicked == false) && (itemSingleClicked == false) && (!selected))
                {
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[1].ForeColor = Color.Gray;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[5].ForeColor = Color.Gray;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[6].ForeColor = Color.Gray;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[7].ForeColor = Color.Gray;
                }

                if (itemSingleClicked == true)
                {
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[1].ForeColor = Color.Black;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[5].ForeColor = Color.Gray;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[6].ForeColor = Color.Black;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[7].ForeColor = Color.Black;

                    itemSingleClicked = false;

                    Console.WriteLine("Mouse single clicked - WndProc");
                }

                if (itemDoubleClicked == true)
                {
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[1].ForeColor = Color.Black;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[5].ForeColor = Color.Gray;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[6].ForeColor = Color.Black;
                    ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[7].ForeColor = Color.Black;


                   // itemSingleClicked = false;
                    itemDoubleClicked = false;

                    Console.WriteLine("Mouse double clicked - WndProc");
                }

                passMessageOn = false;
            }

            if (passMessageOn)
                base.WndProc(ref m);
        }

        private void mouseSingleClickedEvent(object sender, MouseEventArgs e)
        {
            ListViewItem theClickedOne = listView1.GetItemAt(e.X, e.Y);
            Console.WriteLine("Se apeleaza single clicked");

            if (numberOfClicks < 2)
            {                
                if (numberOfClicks == 0)
                {
                    firstNameClicked = theClickedOne.Text.Split(' ')[0];
                    firstIndexClicked = theClickedOne.Index;
                }

                numberOfClicks++;
            }
            else
            {
                numberOfClicks = 0;
                firstIndexClicked = -1;
                firstNameClicked = "";
            }

            itemSingleClicked = true;
        }

        private void mouseDoubleClickedEvent(object sender, MouseEventArgs e)
        {
            ListViewItem theClickedOne = listView1.GetItemAt(e.X, e.Y);
            String id = "", status = "";
            itemDoubleClicked = true;

            String name = theClickedOne.Text.Split(' ')[0];
            Console.WriteLine(name);

            foreach(Friend f in Client.friends)
                if (f.name.Equals(name))
                {
                    id = f.id;
                    status = f.status;
                }

            Discussion discussion = new Discussion(id, name, status);

            discussion.Owner = this;
            discussion.Show();
        }

        public void InitializeListView()
        {
            listView1.View = View.Details;

            List<ListViewItem> items = new List<ListViewItem>();

            ColumnHeader columnHeader0 = new ColumnHeader();
            columnHeader0.Text = "Name";
            columnHeader0.Width = -1;

            listView1.Columns.AddRange(new ColumnHeader[] { columnHeader0 });

            List<ListViewGroup> lists = new List<ListViewGroup>();
            int nr = 0;
            foreach (String g in Client.groups)
            {
                lists.Add(new ListViewGroup(g, HorizontalAlignment.Center)); // pun numele grupului
                foreach (Friend f in Client.friends)
                    if (f.group.Equals(g))
                    {
                        items.Add(new ListViewItem(f.name + "          " + f.status, lists[nr]));
                    }
                listView1.Groups.Add(lists[nr]);
                nr++;
            }    

            listView1.Items.AddRange(items.ToArray());

            listView1.MouseClick += new MouseEventHandler(mouseSingleClickedEvent);
            listView1.MouseDoubleClick += new MouseEventHandler(mouseDoubleClickedEvent);
        }

        private void InitializeMenuStrip()
        {
            ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[1].ForeColor = Color.Gray;
            ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[5].ForeColor = Color.Gray;
            ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[6].ForeColor = Color.Gray;
            ((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[7].ForeColor = Color.Gray;
        }

        private void getMenuStripElement(object sender, ToolStripItemClickedEventArgs e)
        {
            string selectedItem = (string)e.ClickedItem.Text;
            Console.WriteLine(selectedItem);
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string selectedItem = (string)comboBox1.SelectedItem;
            Client.status = selectedItem;
            Client.changeStatus(selectedItem);
        }

        private void listView1_SelectedIndexChanged_1(object sender, EventArgs e)
        {
           // itemDoubleClicked = false;
           // itemSingleClicked = false;
            Console.WriteLine("Se apeleaza selected");
            Console.WriteLine("Number of Clicks: {0}", numberOfClicks);
            if ((numberOfClicks == 2) || ((numberOfClicks == 1) && (listView1.SelectedItems.Count == 0)))
            {
                if (listView1.Items[firstIndexClicked].Text != firstNameClicked)
                    Console.WriteLine("S-a schimbat numele {0} in {1}", firstNameClicked, listView1.Items[firstIndexClicked].Text);

                numberOfClicks = 0;
                firstIndexClicked = -1;
                firstNameClicked = "";
            }
        }

        private void Form_Close(object sender, FormClosedEventArgs e)
        {
            this.Owner.Show();
            Client.logOut();
        }

        private void FriendsList_Load(object sender, EventArgs e)
        {

        }

        private void removeGroupToolStripMenuItem1_Click(object sender, EventArgs e)
        {

            if (((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[5].ForeColor == Color.Black &&
                !listView1.SelectedItems[0].Group.Name.Equals("Friends"))
                Client.deleteGroup(listView1.SelectedItems[0].Group.Header);
        }

        private void menuToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {

        }

        private void menuToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem clickedMenuItem = sender as ToolStripMenuItem;
            if (i == 0)
            {
                clickedMenuItem.DropDownItemClicked += new ToolStripItemClickedEventHandler(getMenuStripElement);
                i++;
            }
        }

        private void editProfileToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            ViewOwnProfile viewProfile = new ViewOwnProfile(Client.id);
            viewProfile.Owner = this;
            viewProfile.Show();
        }

        private void displayProfileToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (((ToolStripMenuItem)(menuStrip1.Items[0])).DropDownItems[1].ForeColor == Color.Gray)
            {
                
            }
        }

        private void addGroupToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            CreateGroup createGroup = new CreateGroup(listView1);
            createGroup.Owner = this;
            createGroup.Show();
        }

        private void addToGroupToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count >= 1)
            {
                string selectedItem = (string)listView1.SelectedItems[0].Text;
                AddUser addUser = new AddUser(listView1, firstNameClicked);
                addUser.Owner = this;
                addUser.Show();
            }
        }

        private void removeFromGroupToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count >= 1)
            {
                string selectedItem = (string)listView1.SelectedItems[0].Text;
                Client.groupDelFriend(firstNameClicked);
            }
        }

        private void signOutToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            FindFriend findFriend = new FindFriend(listView1);
            findFriend.Owner = this;
            findFriend.Show();
        }
    }
}

public class threadparams
{
    public String name, namepartner, message, status;

    public threadparams(String n, String np, String m, String s)
    {
        name = n;
        namepartner = np;
        message = m;
        status = s;
    }
}