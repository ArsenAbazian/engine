namespace sceneEditor.Controls {
    partial class VectorControl {
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
            this.v0 = new System.Windows.Forms.NumericUpDown();
            this.v1 = new System.Windows.Forms.NumericUpDown();
            this.v2 = new System.Windows.Forms.NumericUpDown();
            this.v3 = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // v0
            // 
            this.v0.Location = new System.Drawing.Point(3, 19);
            this.v0.Name = "v0";
            this.v0.Size = new System.Drawing.Size(62, 20);
            this.v0.TabIndex = 0;
            this.v0.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // v1
            // 
            this.v1.Location = new System.Drawing.Point(72, 19);
            this.v1.Name = "v1";
            this.v1.Size = new System.Drawing.Size(62, 20);
            this.v1.TabIndex = 1;
            this.v1.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // v2
            // 
            this.v2.Location = new System.Drawing.Point(139, 19);
            this.v2.Name = "v2";
            this.v2.Size = new System.Drawing.Size(62, 20);
            this.v2.TabIndex = 2;
            this.v2.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // v3
            // 
            this.v3.Location = new System.Drawing.Point(207, 19);
            this.v3.Name = "v3";
            this.v3.Size = new System.Drawing.Size(62, 20);
            this.v3.TabIndex = 3;
            this.v3.Visible = false;
            this.v3.ValueChanged += new System.EventHandler(this.ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(25, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(19, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "[0]";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(94, 3);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(19, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "[1]";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(161, 3);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(19, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "[2]";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(229, 3);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(19, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "[3]";
            this.label4.Visible = false;
            // 
            // VectorControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.v3);
            this.Controls.Add(this.v2);
            this.Controls.Add(this.v1);
            this.Controls.Add(this.v0);
            this.Name = "VectorControl";
            this.Size = new System.Drawing.Size(204, 42);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown v0;
        private System.Windows.Forms.NumericUpDown v1;
        private System.Windows.Forms.NumericUpDown v2;
        private System.Windows.Forms.NumericUpDown v3;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
    }
}
