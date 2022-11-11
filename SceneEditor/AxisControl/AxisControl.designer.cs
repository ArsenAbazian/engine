namespace sceneEditor {
	partial class AxisControl {
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing) {
			if(disposing && (components != null)) {
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
            this.deltaCombo = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.zControl = new sceneEditor.TrackValueControl();
            this.yControl = new sceneEditor.TrackValueControl();
            this.xControl = new sceneEditor.TrackValueControl();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.deltaCombo);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.zControl);
            this.groupBox1.Controls.Add(this.yControl);
            this.groupBox1.Controls.Add(this.xControl);
            this.groupBox1.Location = new System.Drawing.Point(0, 0);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(402, 92);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Global ";
            // 
            // deltaCombo
            // 
            this.deltaCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.deltaCombo.FormattingEnabled = true;
            this.deltaCombo.Items.AddRange(new object[] {
            "1000",
            "100",
            "10",
            "1",
            "0.1",
            "0.01",
            "0.001",
            "0.0001"});
            this.deltaCombo.Location = new System.Drawing.Point(333, 40);
            this.deltaCombo.Name = "deltaCombo";
            this.deltaCombo.Size = new System.Drawing.Size(63, 21);
            this.deltaCombo.TabIndex = 15;
            this.deltaCombo.SelectedIndexChanged += new System.EventHandler(this.deltaCombo_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(330, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(32, 13);
            this.label1.TabIndex = 14;
            this.label1.Text = "Delta";
            // 
            // zControl
            // 
            this.zControl.Delta = 0F;
            this.zControl.Location = new System.Drawing.Point(3, 65);
            this.zControl.Name = "zControl";
            this.zControl.Size = new System.Drawing.Size(321, 21);
            this.zControl.TabIndex = 13;
            this.zControl.Value = 0F;
            this.zControl.ValueName = "Z:";
            // 
            // yControl
            // 
            this.yControl.Delta = 0F;
            this.yControl.Location = new System.Drawing.Point(3, 40);
            this.yControl.Name = "yControl";
            this.yControl.Size = new System.Drawing.Size(321, 21);
            this.yControl.TabIndex = 12;
            this.yControl.Value = 0F;
            this.yControl.ValueName = "Y:";
            // 
            // xControl
            // 
            this.xControl.Delta = 0F;
            this.xControl.Location = new System.Drawing.Point(3, 14);
            this.xControl.Name = "xControl";
            this.xControl.Size = new System.Drawing.Size(321, 21);
            this.xControl.TabIndex = 11;
            this.xControl.Value = 0F;
            this.xControl.ValueName = "X:";
            // 
            // AxisControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox1);
            this.Name = "AxisControl";
            this.Size = new System.Drawing.Size(405, 93);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private TrackValueControl zControl;
		private TrackValueControl yControl;
		private TrackValueControl xControl;
		private System.Windows.Forms.ComboBox deltaCombo;
		private System.Windows.Forms.Label label1;
	}
}
