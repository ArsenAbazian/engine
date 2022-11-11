namespace sceneEditor.Dependency {
    partial class GraphDependencyPropertyControl {
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
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.operationCombo = new System.Windows.Forms.ComboBox();
            this.loopGraph2 = new System.Windows.Forms.CheckBox();
            this.loopGraph1 = new System.Windows.Forms.CheckBox();
            this.clear = new System.Windows.Forms.Button();
            this.newSourceButton = new System.Windows.Forms.Button();
            this.sourceListBox = new System.Windows.Forms.ListBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.graphControl1 = new sceneEditor.graphViewer.GraphControl();
            this.groupBox1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.groupBox3);
            this.groupBox1.Controls.Add(this.loopGraph2);
            this.groupBox1.Controls.Add(this.loopGraph1);
            this.groupBox1.Controls.Add(this.clear);
            this.groupBox1.Controls.Add(this.newSourceButton);
            this.groupBox1.Controls.Add(this.sourceListBox);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(160, 230);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Input";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.operationCombo);
            this.groupBox3.Location = new System.Drawing.Point(6, 176);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(148, 48);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = " ResultType ";
            // 
            // operationCombo
            // 
            this.operationCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.operationCombo.FormattingEnabled = true;
            this.operationCombo.Items.AddRange(new object[] {
            "none",
            "select source 1",
            "select source 2",
            "add",
            "sub",
            "mul",
            "div"});
            this.operationCombo.Location = new System.Drawing.Point(6, 19);
            this.operationCombo.Name = "operationCombo";
            this.operationCombo.Size = new System.Drawing.Size(136, 21);
            this.operationCombo.TabIndex = 0;
            // 
            // loopGraph2
            // 
            this.loopGraph2.AutoSize = true;
            this.loopGraph2.Location = new System.Drawing.Point(6, 153);
            this.loopGraph2.Name = "loopGraph2";
            this.loopGraph2.Size = new System.Drawing.Size(126, 17);
            this.loopGraph2.TabIndex = 4;
            this.loopGraph2.Text = "Loop Source Value 2";
            this.loopGraph2.UseVisualStyleBackColor = true;
            // 
            // loopGraph1
            // 
            this.loopGraph1.AutoSize = true;
            this.loopGraph1.Location = new System.Drawing.Point(6, 130);
            this.loopGraph1.Name = "loopGraph1";
            this.loopGraph1.Size = new System.Drawing.Size(126, 17);
            this.loopGraph1.TabIndex = 3;
            this.loopGraph1.Text = "Loop Source Value 1";
            this.loopGraph1.UseVisualStyleBackColor = true;
            // 
            // clear
            // 
            this.clear.Enabled = false;
            this.clear.Location = new System.Drawing.Point(6, 92);
            this.clear.Name = "clear";
            this.clear.Size = new System.Drawing.Size(148, 23);
            this.clear.TabIndex = 2;
            this.clear.Text = "Clear Source";
            this.clear.UseVisualStyleBackColor = true;
            this.clear.Click += new System.EventHandler(this.clear_Click);
            // 
            // newSourceButton
            // 
            this.newSourceButton.Enabled = false;
            this.newSourceButton.Location = new System.Drawing.Point(6, 68);
            this.newSourceButton.Name = "newSourceButton";
            this.newSourceButton.Size = new System.Drawing.Size(148, 23);
            this.newSourceButton.TabIndex = 1;
            this.newSourceButton.Text = "Set New Source";
            this.newSourceButton.UseVisualStyleBackColor = true;
            this.newSourceButton.Click += new System.EventHandler(this.newSourceButton_Click);
            // 
            // sourceListBox
            // 
            this.sourceListBox.FormattingEnabled = true;
            this.sourceListBox.Items.AddRange(new object[] {
            "Select Source Property 1",
            "Select Source Property 2"});
            this.sourceListBox.Location = new System.Drawing.Point(6, 19);
            this.sourceListBox.Name = "sourceListBox";
            this.sourceListBox.Size = new System.Drawing.Size(148, 43);
            this.sourceListBox.TabIndex = 1;
            this.sourceListBox.SelectedIndexChanged += new System.EventHandler(this.sourceListBox_SelectedIndexChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.graphControl1);
            this.groupBox2.Location = new System.Drawing.Point(169, 3);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(687, 411);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Graph";
            // 
            // graphControl1
            // 
            this.graphControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.graphControl1.Location = new System.Drawing.Point(3, 16);
            this.graphControl1.Name = "graphControl1";
            this.graphControl1.Padding = new System.Windows.Forms.Padding(2);
            this.graphControl1.Size = new System.Drawing.Size(681, 392);
            this.graphControl1.TabIndex = 0;
            // 
            // GraphDependencyPropertyControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "GraphDependencyPropertyControl";
            this.Size = new System.Drawing.Size(859, 417);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button newSourceButton;
        private System.Windows.Forms.ListBox sourceListBox;
        private System.Windows.Forms.GroupBox groupBox2;
        private sceneEditor.graphViewer.GraphControl graphControl1;
        private System.Windows.Forms.Button clear;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.ComboBox operationCombo;
        private System.Windows.Forms.CheckBox loopGraph2;
        private System.Windows.Forms.CheckBox loopGraph1;
    }
}
