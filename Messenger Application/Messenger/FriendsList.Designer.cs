namespace Messenger
{
    partial class FriendsList
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.listView1 = new System.Windows.Forms.ListView();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.menuToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editProfileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.displayProfileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addToGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeFromGroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.signOutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editProfileToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.displayProfileToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.addGroupToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.removeGroupToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.addToGroupToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.removeFromGroupToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.signOutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.menuToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "Available",
            "Busy",
            "Not at my desk",
            "Away"});
            this.comboBox1.Location = new System.Drawing.Point(118, 53);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(170, 21);
            this.comboBox1.TabIndex = 0;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 56);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = Client.id;
            // 
            // listView1
            // 
            this.listView1.Alignment = System.Windows.Forms.ListViewAlignment.Left;
            this.listView1.Cursor = System.Windows.Forms.Cursors.Hand;
            this.listView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.listView1.LabelEdit = true;
            this.listView1.Location = new System.Drawing.Point(15, 100);
            this.listView1.MultiSelect = false;
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(273, 375);
            this.listView1.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.listView1.TabIndex = 4;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged_1);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.menuToolStripMenuItem1});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(305, 24);
            this.menuStrip1.TabIndex = 5;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.ItemClicked += new System.Windows.Forms.ToolStripItemClickedEventHandler(this.menuStrip1_ItemClicked);
            // 
            // menuToolStripMenuItem
            // 
            this.menuToolStripMenuItem.Name = "menuToolStripMenuItem";
            this.menuToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.menuToolStripMenuItem.Text = "Menu";
            this.menuToolStripMenuItem.Click += new System.EventHandler(this.menuToolStripMenuItem_Click);
            // 
            // editProfileToolStripMenuItem
            // 
            this.editProfileToolStripMenuItem.Name = "editProfileToolStripMenuItem";
            this.editProfileToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.editProfileToolStripMenuItem.Text = "Edit Profile";
            // 
            // displayProfileToolStripMenuItem
            // 
            this.displayProfileToolStripMenuItem.Name = "displayProfileToolStripMenuItem";
            this.displayProfileToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.displayProfileToolStripMenuItem.Text = "Display Profile";
            // 
            // addGroupToolStripMenuItem
            // 
            this.addGroupToolStripMenuItem.Name = "addGroupToolStripMenuItem";
            this.addGroupToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.addGroupToolStripMenuItem.Text = "Add Group";
            // 
            // removeGroupToolStripMenuItem
            // 
            this.removeGroupToolStripMenuItem.Name = "removeGroupToolStripMenuItem";
            this.removeGroupToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.removeGroupToolStripMenuItem.Text = "Remove Group";
            this.removeGroupToolStripMenuItem.Click += new System.EventHandler(this.removeGroupToolStripMenuItem1_Click);
            // 
            // addToGroupToolStripMenuItem
            // 
            this.addToGroupToolStripMenuItem.Name = "addToGroupToolStripMenuItem";
            this.addToGroupToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.addToGroupToolStripMenuItem.Text = "Add to Group";
            // 
            // removeFromGroupToolStripMenuItem
            // 
            this.removeFromGroupToolStripMenuItem.Name = "removeFromGroupToolStripMenuItem";
            this.removeFromGroupToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.removeFromGroupToolStripMenuItem.Text = "Remove from Group";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(179, 6);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(179, 6);
            // 
            // signOutToolStripMenuItem
            // 
            this.signOutToolStripMenuItem.Name = "signOutToolStripMenuItem";
            this.signOutToolStripMenuItem.Size = new System.Drawing.Size(182, 22);
            this.signOutToolStripMenuItem.Text = "Sign out";
            // 
            // editProfileToolStripMenuItem1
            // 
            this.editProfileToolStripMenuItem1.Name = "editProfileToolStripMenuItem1";
            this.editProfileToolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.editProfileToolStripMenuItem1.Text = "Edit Profile";
            this.editProfileToolStripMenuItem1.Click += new System.EventHandler(this.editProfileToolStripMenuItem1_Click);
            // 
            // displayProfileToolStripMenuItem1
            // 
            this.displayProfileToolStripMenuItem1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.displayProfileToolStripMenuItem1.Name = "displayProfileToolStripMenuItem1";
            this.displayProfileToolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.displayProfileToolStripMenuItem1.Text = "View Profile";
            this.displayProfileToolStripMenuItem1.Click += new System.EventHandler(this.displayProfileToolStripMenuItem1_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(179, 6);
            // 
            // addGroupToolStripMenuItem1
            // 
            this.addGroupToolStripMenuItem1.Name = "addGroupToolStripMenuItem1";
            this.addGroupToolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.addGroupToolStripMenuItem1.Text = "Add Group";
            this.addGroupToolStripMenuItem1.Click += new System.EventHandler(this.addGroupToolStripMenuItem1_Click);
            // 
            // removeGroupToolStripMenuItem1
            // 
            this.removeGroupToolStripMenuItem1.Name = "removeGroupToolStripMenuItem1";
            this.removeGroupToolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.removeGroupToolStripMenuItem1.Text = "Remove Group";
            this.removeGroupToolStripMenuItem1.Click += new System.EventHandler(this.removeGroupToolStripMenuItem1_Click);
            // 
            // addToGroupToolStripMenuItem1
            // 
            this.addToGroupToolStripMenuItem1.Name = "addToGroupToolStripMenuItem1";
            this.addToGroupToolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.addToGroupToolStripMenuItem1.Text = "Add to Group";
            this.addToGroupToolStripMenuItem1.Click += new System.EventHandler(this.addToGroupToolStripMenuItem1_Click);
            // 
            // removeFromGroupToolStripMenuItem1
            // 
            this.removeFromGroupToolStripMenuItem1.Name = "removeFromGroupToolStripMenuItem1";
            this.removeFromGroupToolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.removeFromGroupToolStripMenuItem1.Text = "Remove from Group";
            this.removeFromGroupToolStripMenuItem1.Click += new System.EventHandler(this.removeFromGroupToolStripMenuItem1_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(179, 6);
            // 
            // signOutToolStripMenuItem1
            // 
            this.signOutToolStripMenuItem1.Name = "signOutToolStripMenuItem1";
            this.signOutToolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.signOutToolStripMenuItem1.Text = "Sign out";
            this.signOutToolStripMenuItem1.Click += new System.EventHandler(this.signOutToolStripMenuItem1_Click);
            // 
            // menuToolStripMenuItem1
            // 
            this.menuToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.editProfileToolStripMenuItem1,
            this.displayProfileToolStripMenuItem1,
            this.toolStripMenuItem1,
            this.toolStripSeparator3,
            this.addGroupToolStripMenuItem1,
            this.removeGroupToolStripMenuItem1,
            this.addToGroupToolStripMenuItem1,
            this.removeFromGroupToolStripMenuItem1,
            this.toolStripSeparator4,
            this.signOutToolStripMenuItem1});
            this.menuToolStripMenuItem1.Name = "menuToolStripMenuItem1";
            this.menuToolStripMenuItem1.Size = new System.Drawing.Size(50, 20);
            this.menuToolStripMenuItem1.Text = "Menu";
            this.menuToolStripMenuItem1.Click += new System.EventHandler(this.menuToolStripMenuItem1_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(182, 22);
            this.toolStripMenuItem1.Text = "Find Friend";
            this.toolStripMenuItem1.Click += new System.EventHandler(this.toolStripMenuItem1_Click);
            // 
            // FriendsList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(305, 487);
            this.Controls.Add(this.listView1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "FriendsList";
            this.Text = "Nano Messenger";
            this.Load += new System.EventHandler(this.FriendsList_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView listView1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem menuToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editProfileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem displayProfileToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem addGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addToGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeFromGroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem signOutToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem menuToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem editProfileToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem displayProfileToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem addGroupToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem removeGroupToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem addToGroupToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem removeFromGroupToolStripMenuItem1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem signOutToolStripMenuItem1;
    }
}

