namespace sceneEditor.Dependency {
    partial class DependencyValueControl {
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
            this.valueTextBox1 = new sceneEditor.Controls.ValueTextBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // valueTextBox1
            // 
            this.valueTextBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.valueTextBox1.Location = new System.Drawing.Point(0, 0);
            this.valueTextBox1.Name = "valueTextBox1";
            this.valueTextBox1.Size = new System.Drawing.Size(215, 20);
            this.valueTextBox1.TabIndex = 0;
            this.valueTextBox1.ValueChanged += new System.EventHandler(this.valueTextBox1_ValueChanged);
            // 
            // comboBox1
            // 
            this.comboBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.comboBox1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "True",
            "False"});
            this.comboBox1.Location = new System.Drawing.Point(0, 0);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(215, 21);
            this.comboBox1.TabIndex = 1;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // DependencyValueControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.valueTextBox1);
            this.Name = "DependencyValueControl";
            this.Size = new System.Drawing.Size(215, 21);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private sceneEditor.Controls.ValueTextBox valueTextBox1;
        private System.Windows.Forms.ComboBox comboBox1;
    }
}
