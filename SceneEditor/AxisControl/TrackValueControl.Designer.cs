namespace sceneEditor {
	partial class TrackValueControl {
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
            this.label1 = new System.Windows.Forms.Label();
            this.valueText = new System.Windows.Forms.TextBox();
            this.valueTrack = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.valueTrack)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(17, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "X:";
            // 
            // valueText
            // 
            this.valueText.Location = new System.Drawing.Point(27, 0);
            this.valueText.Name = "valueText";
            this.valueText.Size = new System.Drawing.Size(100, 20);
            this.valueText.TabIndex = 3;
            this.valueText.Text = "0";
            this.valueText.KeyDown += new System.Windows.Forms.KeyEventHandler(this.valueText_KeyDown);
            // 
            // valueTrack
            // 
            this.valueTrack.AutoSize = false;
            this.valueTrack.Location = new System.Drawing.Point(133, 0);
            this.valueTrack.Maximum = 1000;
            this.valueTrack.Name = "valueTrack";
            this.valueTrack.Size = new System.Drawing.Size(184, 20);
            this.valueTrack.TabIndex = 5;
            this.valueTrack.Value = 500;
            this.valueTrack.MouseDown += new System.Windows.Forms.MouseEventHandler(this.valueTrack_MouseDown);
            this.valueTrack.ValueChanged += new System.EventHandler(this.valueTrack_ValueChanged);
            this.valueTrack.MouseUp += new System.Windows.Forms.MouseEventHandler(this.valueTrack_MouseUp);
            // 
            // TrackValueControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.valueTrack);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.valueText);
            this.Name = "TrackValueControl";
            this.Size = new System.Drawing.Size(321, 21);
            ((System.ComponentModel.ISupportInitialize)(this.valueTrack)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox valueText;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TrackBar valueTrack;
	}
}
