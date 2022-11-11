namespace sceneEditor {
	partial class SettingsControl {
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
            this.objectProp = new System.Windows.Forms.PropertyGrid();
            this.objectList = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // objectProp
            // 
            this.objectProp.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.objectProp.Location = new System.Drawing.Point(3, 30);
            this.objectProp.Name = "objectProp";
            this.objectProp.Size = new System.Drawing.Size(246, 314);
            this.objectProp.TabIndex = 0;
            this.objectProp.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.objectProp_PropertyValueChanged);
            // 
            // objectList
            // 
            this.objectList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.objectList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.objectList.FormattingEnabled = true;
            this.objectList.Location = new System.Drawing.Point(3, 3);
            this.objectList.Name = "objectList";
            this.objectList.Size = new System.Drawing.Size(246, 21);
            this.objectList.TabIndex = 1;
            this.objectList.SelectedIndexChanged += new System.EventHandler(this.objectList_SelectedIndexChanged);
            // 
            // SettingsControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.objectList);
            this.Controls.Add(this.objectProp);
            this.Name = "SettingsControl";
            this.Size = new System.Drawing.Size(252, 347);
            this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.PropertyGrid objectProp;
		private System.Windows.Forms.ComboBox objectList;
	}
}
