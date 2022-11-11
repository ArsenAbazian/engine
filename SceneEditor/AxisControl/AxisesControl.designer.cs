namespace sceneEditor {
	partial class AxisesControl {
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
            this.localAxisControl = new sceneEditor.AxisControl();
            this.globalAxisControl = new sceneEditor.AxisControl();
            this.axisControl2 = new sceneEditor.AxisControl();
            this.SuspendLayout();
            // 
            // localAxisControl
            // 
            this.localAxisControl.Delta = 1F;
            this.localAxisControl.GroupText = " Global ";
            this.localAxisControl.Location = new System.Drawing.Point(0, 99);
            this.localAxisControl.Name = "localAxisControl";
            this.localAxisControl.Size = new System.Drawing.Size(405, 93);
            this.localAxisControl.TabIndex = 1;
            this.localAxisControl.XValue = 0F;
            this.localAxisControl.YValue = 0F;
            this.localAxisControl.ZValue = 0F;
            // 
            // globalAxisControl
            // 
            this.globalAxisControl.Delta = 1F;
            this.globalAxisControl.GroupText = " Global ";
            this.globalAxisControl.Location = new System.Drawing.Point(0, 0);
            this.globalAxisControl.Name = "globalAxisControl";
            this.globalAxisControl.Size = new System.Drawing.Size(405, 93);
            this.globalAxisControl.TabIndex = 0;
            this.globalAxisControl.XValue = 0F;
            this.globalAxisControl.YValue = 0F;
            this.globalAxisControl.ZValue = 0F;
            // 
            // axisControl2
            // 
            this.axisControl2.Delta = 1F;
            this.axisControl2.GroupText = " Global ";
            this.axisControl2.Location = new System.Drawing.Point(0, 99);
            this.axisControl2.Name = "axisControl2";
            this.axisControl2.Size = new System.Drawing.Size(405, 93);
            this.axisControl2.TabIndex = 1;
            this.axisControl2.XValue = 0F;
            this.axisControl2.YValue = 0F;
            this.axisControl2.ZValue = 0F;
            // 
            // AxisesControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.localAxisControl);
            this.Controls.Add(this.globalAxisControl);
            this.Name = "AxisesControl";
            this.Size = new System.Drawing.Size(402, 193);
            this.ResumeLayout(false);

		}

		#endregion

		private AxisControl globalAxisControl;
		private AxisControl localAxisControl;
        private AxisControl axisControl2;
	}
}
