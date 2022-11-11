namespace sceneEditor {
	partial class AxisesForm {
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
			this.axisesControl1 = new sceneEditor.AxisesControl();
			this.SuspendLayout();
			// 
			// axisesControl1
			// 
			this.axisesControl1.Location = new System.Drawing.Point(3, 3);
			this.axisesControl1.Name = "axisesControl1";
			this.axisesControl1.Size = new System.Drawing.Size(402, 192);
			this.axisesControl1.TabIndex = 0;
			// 
			// AxisesForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(408, 196);
			this.Controls.Add(this.axisesControl1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "AxisesForm";
			this.Text = "AxisesForm";
			this.TopMost = true;
			this.ResumeLayout(false);

		}

		#endregion

		private AxisesControl axisesControl1;
	}
}