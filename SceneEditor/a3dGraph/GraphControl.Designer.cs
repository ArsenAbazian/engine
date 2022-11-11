namespace sceneEditor.graphViewer {
    partial class GraphControl {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(GraphControl));
            this.graphColorDialog = new System.Windows.Forms.ColorDialog();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.xCoor = new sceneEditor.Controls.FloatValueControl();
            this.yCoor = new sceneEditor.Controls.FloatValueControl();
            this.widthEdit = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.nodeTypeCombo = new System.Windows.Forms.ComboBox();
            this.graphName = new System.Windows.Forms.TextBox();
            this.visibleCheck = new System.Windows.Forms.CheckBox();
            this.graphComboBox = new System.Windows.Forms.ComboBox();
            this.nodeButton = new System.Windows.Forms.Button();
            this.graphButton = new System.Windows.Forms.Button();
            this.graphMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addGraphMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeGraphMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.nodeMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.removeNodeMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.joinNodeToNextMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.breakOffNodeMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.graphViewer = new sceneEditor.graphViewer.GraphViewer();
            this.controlPropertiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1.SuspendLayout();
            this.graphMenu.SuspendLayout();
            this.nodeMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ControlDark;
            this.panel1.Controls.Add(this.label5);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.xCoor);
            this.panel1.Controls.Add(this.yCoor);
            this.panel1.Controls.Add(this.widthEdit);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.nodeTypeCombo);
            this.panel1.Controls.Add(this.graphName);
            this.panel1.Controls.Add(this.visibleCheck);
            this.panel1.Controls.Add(this.graphComboBox);
            this.panel1.Controls.Add(this.nodeButton);
            this.panel1.Controls.Add(this.graphButton);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(2, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(624, 53);
            this.panel1.TabIndex = 4;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.ForeColor = System.Drawing.Color.White;
            this.label5.Location = new System.Drawing.Point(143, 7);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(70, 13);
            this.label5.TabIndex = 14;
            this.label5.Text = "Graph Name:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.ForeColor = System.Drawing.Color.White;
            this.label4.Location = new System.Drawing.Point(497, 30);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(17, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Y:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.ForeColor = System.Drawing.Color.White;
            this.label3.Location = new System.Drawing.Point(356, 30);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(17, 13);
            this.label3.TabIndex = 12;
            this.label3.Text = "X:";
            // 
            // xCoor
            // 
            this.xCoor.Location = new System.Drawing.Point(379, 27);
            this.xCoor.Name = "xCoor";
            this.xCoor.Size = new System.Drawing.Size(100, 20);
            this.xCoor.TabIndex = 11;
            this.xCoor.ValueChanged += new System.EventHandler(this.xCoor_ValueChanged);
            // 
            // yCoor
            // 
            this.yCoor.Location = new System.Drawing.Point(520, 27);
            this.yCoor.Name = "yCoor";
            this.yCoor.Size = new System.Drawing.Size(100, 20);
            this.yCoor.TabIndex = 10;
            this.yCoor.ValueChanged += new System.EventHandler(this.yCoor_ValueChanged);
            // 
            // widthEdit
            // 
            this.widthEdit.Location = new System.Drawing.Point(217, 27);
            this.widthEdit.Name = "widthEdit";
            this.widthEdit.Size = new System.Drawing.Size(48, 20);
            this.widthEdit.TabIndex = 9;
            this.widthEdit.KeyDown += new System.Windows.Forms.KeyEventHandler(this.widthEdit_KeyDown);
            this.widthEdit.Leave += new System.EventHandler(this.widthEdit_Leave);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.White;
            this.label2.Location = new System.Drawing.Point(175, 32);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(38, 13);
            this.label2.TabIndex = 8;
            this.label2.Text = "Width:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(8, 33);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(63, 13);
            this.label1.TabIndex = 6;
            this.label1.Text = "Node Type:";
            // 
            // nodeTypeCombo
            // 
            this.nodeTypeCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.nodeTypeCombo.FormattingEnabled = true;
            this.nodeTypeCombo.Items.AddRange(new object[] {
            "Const",
            "Line",
            "BezieSpline",
            "Sin"});
            this.nodeTypeCombo.Location = new System.Drawing.Point(77, 27);
            this.nodeTypeCombo.Name = "nodeTypeCombo";
            this.nodeTypeCombo.Size = new System.Drawing.Size(84, 21);
            this.nodeTypeCombo.TabIndex = 5;
            this.nodeTypeCombo.SelectedIndexChanged += new System.EventHandler(this.nodeTypeCombo_SelectedIndexChanged);
            // 
            // graphName
            // 
            this.graphName.Location = new System.Drawing.Point(217, 4);
            this.graphName.Name = "graphName";
            this.graphName.Size = new System.Drawing.Size(80, 20);
            this.graphName.TabIndex = 4;
            this.graphName.KeyDown += new System.Windows.Forms.KeyEventHandler(this.graphName_KeyDown);
            this.graphName.Leave += new System.EventHandler(this.graphName_Leave);
            // 
            // visibleCheck
            // 
            this.visibleCheck.AutoSize = true;
            this.visibleCheck.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.visibleCheck.ForeColor = System.Drawing.Color.White;
            this.visibleCheck.Location = new System.Drawing.Point(317, 6);
            this.visibleCheck.Name = "visibleCheck";
            this.visibleCheck.Size = new System.Drawing.Size(56, 17);
            this.visibleCheck.TabIndex = 3;
            this.visibleCheck.Text = "Visible";
            this.visibleCheck.UseVisualStyleBackColor = true;
            this.visibleCheck.CheckedChanged += new System.EventHandler(this.visibleCheck_CheckedChanged);
            // 
            // graphComboBox
            // 
            this.graphComboBox.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            this.graphComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.graphComboBox.FormattingEnabled = true;
            this.graphComboBox.Location = new System.Drawing.Point(379, 4);
            this.graphComboBox.Name = "graphComboBox";
            this.graphComboBox.Size = new System.Drawing.Size(241, 21);
            this.graphComboBox.TabIndex = 2;
            this.graphComboBox.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.graphComboBox_DrawItem);
            this.graphComboBox.SelectedIndexChanged += new System.EventHandler(this.graphComboBox_SelectedIndexChanged);
            this.graphComboBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this.graphComboBox_MouseDown);
            // 
            // nodeButton
            // 
            this.nodeButton.BackColor = System.Drawing.SystemColors.Control;
            this.nodeButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.nodeButton.Location = new System.Drawing.Point(56, 3);
            this.nodeButton.Name = "nodeButton";
            this.nodeButton.Size = new System.Drawing.Size(47, 21);
            this.nodeButton.TabIndex = 1;
            this.nodeButton.Text = "Node";
            this.nodeButton.UseVisualStyleBackColor = false;
            this.nodeButton.Click += new System.EventHandler(this.nodeButton_Click);
            // 
            // graphButton
            // 
            this.graphButton.BackColor = System.Drawing.SystemColors.Control;
            this.graphButton.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
            this.graphButton.Location = new System.Drawing.Point(3, 3);
            this.graphButton.Name = "graphButton";
            this.graphButton.Size = new System.Drawing.Size(47, 21);
            this.graphButton.TabIndex = 0;
            this.graphButton.Text = "Graph";
            this.graphButton.UseVisualStyleBackColor = false;
            this.graphButton.MouseDown += new System.Windows.Forms.MouseEventHandler(this.graphButton_MouseDown);
            // 
            // graphMenu
            // 
            this.graphMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addGraphMenuItem,
            this.removeGraphMenuItem});
            this.graphMenu.Name = "graphMenu";
            this.graphMenu.Size = new System.Drawing.Size(157, 48);
            // 
            // addGraphMenuItem
            // 
            this.addGraphMenuItem.Name = "addGraphMenuItem";
            this.addGraphMenuItem.Size = new System.Drawing.Size(156, 22);
            this.addGraphMenuItem.Text = "Add Graph";
            this.addGraphMenuItem.Click += new System.EventHandler(this.addNewGraphMenuItem_Click);
            // 
            // removeGraphMenuItem
            // 
            this.removeGraphMenuItem.Name = "removeGraphMenuItem";
            this.removeGraphMenuItem.Size = new System.Drawing.Size(156, 22);
            this.removeGraphMenuItem.Text = "Remove Graph";
            // 
            // nodeMenu
            // 
            this.nodeMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.removeNodeMenuItem,
            this.joinNodeToNextMenuItem,
            this.breakOffNodeMenuItem,
            this.controlPropertiesToolStripMenuItem});
            this.nodeMenu.Name = "nodeMenu";
            this.nodeMenu.Size = new System.Drawing.Size(173, 114);
            // 
            // removeNodeMenuItem
            // 
            this.removeNodeMenuItem.Name = "removeNodeMenuItem";
            this.removeNodeMenuItem.Size = new System.Drawing.Size(172, 22);
            this.removeNodeMenuItem.Text = "Remove Node";
            this.removeNodeMenuItem.Click += new System.EventHandler(this.removeNodeMenuItem_Click);
            // 
            // joinNodeToNextMenuItem
            // 
            this.joinNodeToNextMenuItem.Name = "joinNodeToNextMenuItem";
            this.joinNodeToNextMenuItem.Size = new System.Drawing.Size(172, 22);
            this.joinNodeToNextMenuItem.Text = "Join Node To next";
            this.joinNodeToNextMenuItem.Click += new System.EventHandler(this.joinNodeToNextMenuItem_Click);
            // 
            // breakOffNodeMenuItem
            // 
            this.breakOffNodeMenuItem.Name = "breakOffNodeMenuItem";
            this.breakOffNodeMenuItem.Size = new System.Drawing.Size(172, 22);
            this.breakOffNodeMenuItem.Text = "Break Off Node";
            this.breakOffNodeMenuItem.Click += new System.EventHandler(this.breakOffNodeMenuItem_Click);
            // 
            // graphViewer
            // 
            this.graphViewer.AxisColor = System.Drawing.Color.DarkGray;
            this.graphViewer.BackColor = System.Drawing.Color.White;
            this.graphViewer.ContextMenuStrip = this.nodeMenu;
            this.graphViewer.DefaultOriginPoint = ((System.Drawing.PointF)(resources.GetObject("graphViewer.DefaultOriginPoint")));
            this.graphViewer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.graphViewer.DownPoint = new System.Drawing.Point(-10000, -10000);
            this.graphViewer.GridColor = System.Drawing.Color.LightGray;
            this.graphViewer.GridLineStyle = System.Drawing.Drawing2D.DashStyle.Dash;
            this.graphViewer.HotTrackColor = System.Drawing.Color.Orange;
            this.graphViewer.LabelColor = System.Drawing.Color.DarkGray;
            this.graphViewer.LabelFont = new System.Drawing.Font("Arial", 7F);
            this.graphViewer.Location = new System.Drawing.Point(2, 55);
            this.graphViewer.Name = "graphViewer";
            this.graphViewer.OriginPoint = ((System.Drawing.PointF)(resources.GetObject("graphViewer.OriginPoint")));
            this.graphViewer.PressedColor = System.Drawing.Color.DarkOrange;
            this.graphViewer.Size = new System.Drawing.Size(624, 328);
            this.graphViewer.TabIndex = 3;
            this.graphViewer.Text = "graphViewer1";
            this.graphViewer.TickLength = 5;
            this.graphViewer.ZoneColor2 = System.Drawing.Color.FromArgb(((int)(((byte)(251)))), ((int)(((byte)(233)))), ((int)(((byte)(225)))));
            this.graphViewer.MouseMove += new System.Windows.Forms.MouseEventHandler(this.graphViewer_MouseMove);
            this.graphViewer.MouseDown += new System.Windows.Forms.MouseEventHandler(this.graphViewer_MouseDown);
            this.graphViewer.KeyDown += new System.Windows.Forms.KeyEventHandler(this.graphViewer_KeyDown);
            // 
            // controlPropertiesToolStripMenuItem
            // 
            this.controlPropertiesToolStripMenuItem.Name = "controlPropertiesToolStripMenuItem";
            this.controlPropertiesToolStripMenuItem.Size = new System.Drawing.Size(172, 22);
            this.controlPropertiesToolStripMenuItem.Text = "Control Properties";
            this.controlPropertiesToolStripMenuItem.Click += new System.EventHandler(this.controlPropertiesToolStripMenuItem_Click);
            // 
            // GraphControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.graphViewer);
            this.Controls.Add(this.panel1);
            this.Name = "GraphControl";
            this.Padding = new System.Windows.Forms.Padding(2);
            this.Size = new System.Drawing.Size(628, 385);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.graphMenu.ResumeLayout(false);
            this.nodeMenu.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ColorDialog graphColorDialog;
        private GraphViewer graphViewer;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button nodeButton;
        private System.Windows.Forms.Button graphButton;
        private System.Windows.Forms.ComboBox graphComboBox;
        private System.Windows.Forms.TextBox graphName;
        private System.Windows.Forms.CheckBox visibleCheck;
        private System.Windows.Forms.ContextMenuStrip graphMenu;
        private System.Windows.Forms.ContextMenuStrip nodeMenu;
        private System.Windows.Forms.ToolStripMenuItem addGraphMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeGraphMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeNodeMenuItem;
        private System.Windows.Forms.ToolStripMenuItem joinNodeToNextMenuItem;
        private System.Windows.Forms.ToolStripMenuItem breakOffNodeMenuItem;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox nodeTypeCombo;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox widthEdit;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private sceneEditor.Controls.FloatValueControl xCoor;
        private sceneEditor.Controls.FloatValueControl yCoor;
        private System.Windows.Forms.ToolStripMenuItem controlPropertiesToolStripMenuItem;

    }
}
