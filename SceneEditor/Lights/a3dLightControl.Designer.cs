namespace sceneEditor.Lights {
    partial class a3dLightControl {
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
            classMapper.Vector3_Mapper vector3_Mapper1 = new classMapper.Vector3_Mapper();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.typeList = new System.Windows.Forms.ComboBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.a3dDirectionalLightControl1 = new sceneEditor.Lights.a3dDirectionalLightControl();
            this.a3dOmniLightControl1 = new sceneEditor.Lights.a3dOmniLightControl();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.typeList);
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(236, 44);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = " Type ";
            // 
            // typeList
            // 
            this.typeList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.typeList.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.typeList.FormattingEnabled = true;
            this.typeList.Items.AddRange(new object[] {
            "OmniLight",
            "DirectionalLight"});
            this.typeList.Location = new System.Drawing.Point(6, 17);
            this.typeList.Name = "typeList";
            this.typeList.Size = new System.Drawing.Size(224, 21);
            this.typeList.TabIndex = 0;
            this.typeList.SelectedIndexChanged += new System.EventHandler(this.typeList_SelectedIndexChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.a3dDirectionalLightControl1);
            this.groupBox2.Controls.Add(this.a3dOmniLightControl1);
            this.groupBox2.Location = new System.Drawing.Point(3, 53);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(236, 226);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = " Params ";
            // 
            // a3dDirectionalLightControl1
            // 
            vector3_Mapper1.X = 0F;
            vector3_Mapper1.Y = 0F;
            vector3_Mapper1.Z = 0F;
            this.a3dDirectionalLightControl1.Direction = vector3_Mapper1;
            this.a3dDirectionalLightControl1.DirLight = null;
            this.a3dDirectionalLightControl1.Light = null;
            this.a3dDirectionalLightControl1.LightColor = System.Drawing.Color.Empty;
            this.a3dDirectionalLightControl1.LightName = "";
            this.a3dDirectionalLightControl1.Location = new System.Drawing.Point(6, 19);
            this.a3dDirectionalLightControl1.MaximumSize = new System.Drawing.Size(223, 1000);
            this.a3dDirectionalLightControl1.MinimumSize = new System.Drawing.Size(223, 58);
            this.a3dDirectionalLightControl1.Name = "a3dDirectionalLightControl1";
            this.a3dDirectionalLightControl1.Size = new System.Drawing.Size(223, 124);
            this.a3dDirectionalLightControl1.TabIndex = 1;
            // 
            // a3dOmniLightControl1
            // 
            this.a3dOmniLightControl1.FarAttenEnd = 0F;
            this.a3dOmniLightControl1.FarAttenStart = 0F;
            this.a3dOmniLightControl1.Light = null;
            this.a3dOmniLightControl1.LightColor = System.Drawing.Color.Empty;
            this.a3dOmniLightControl1.LightName = "";
            this.a3dOmniLightControl1.Location = new System.Drawing.Point(6, 19);
            this.a3dOmniLightControl1.MaximumSize = new System.Drawing.Size(223, 1000);
            this.a3dOmniLightControl1.MinimumSize = new System.Drawing.Size(223, 58);
            this.a3dOmniLightControl1.Name = "a3dOmniLightControl1";
            this.a3dOmniLightControl1.OmniLight = null;
            this.a3dOmniLightControl1.Size = new System.Drawing.Size(223, 172);
            this.a3dOmniLightControl1.TabIndex = 0;
            this.a3dOmniLightControl1.Visible = false;
            // 
            // a3dLightControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "a3dLightControl";
            this.Size = new System.Drawing.Size(242, 282);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox typeList;
        private System.Windows.Forms.GroupBox groupBox2;
        private a3dOmniLightControl a3dOmniLightControl1;
        private a3dDirectionalLightControl a3dDirectionalLightControl1;
    }
}
