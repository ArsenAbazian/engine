namespace sceneEditor {
	partial class ObjectPropertyControl {
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
			this.objectProperty = new System.Windows.Forms.PropertyGrid();
			this.SuspendLayout();
			// 
			// objectProperty
			// 
			this.objectProperty.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.objectProperty.Location = new System.Drawing.Point(3, 3);
			this.objectProperty.Name = "objectProperty";
			this.objectProperty.Size = new System.Drawing.Size(209, 311);
			this.objectProperty.TabIndex = 0;
			// 
			// ObjectPropertyControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.objectProperty);
			this.Name = "ObjectPropertyControl";
			this.Size = new System.Drawing.Size(215, 317);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PropertyGrid objectProperty;
	}
}
