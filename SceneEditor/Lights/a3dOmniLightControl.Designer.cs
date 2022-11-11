namespace sceneEditor.Lights {
    partial class a3dOmniLightControl {
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
            this.vectorControl1 = new sceneEditor.Controls.VectorControl();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.farAttenStart = new System.Windows.Forms.NumericUpDown();
            this.farAttenEnd = new System.Windows.Forms.NumericUpDown();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.farAttenStart)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.farAttenEnd)).BeginInit();
            this.SuspendLayout();
            // 
            // lightName
            // 
            this.lightName.Location = new System.Drawing.Point(81, 7);
            this.lightName.Size = new System.Drawing.Size(136, 20);
            // 
            // lightColor
            // 
            this.lightColor.Location = new System.Drawing.Point(81, 33);
            this.lightColor.Size = new System.Drawing.Size(136, 21);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.vectorControl1);
            this.groupBox1.Location = new System.Drawing.Point(1, 112);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(222, 58);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Position ";
            // 
            // vectorControl1
            // 
            this.vectorControl1.Location = new System.Drawing.Point(8, 14);
            this.vectorControl1.Name = "vectorControl1";
            this.vectorControl1.Size = new System.Drawing.Size(204, 42);
            this.vectorControl1.TabIndex = 0;
            this.vectorControl1.V0 = 0F;
            this.vectorControl1.V1 = 0F;
            this.vectorControl1.V2 = 0F;
            this.vectorControl1.V3 = 0F;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 63);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(72, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "FarAttenStart:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 89);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(69, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "FarAttenEnd:";
            // 
            // farAttenStart
            // 
            this.farAttenStart.DecimalPlaces = 5;
            this.farAttenStart.Increment = new decimal(new int[] {
            5,
            0,
            0,
            196608});
            this.farAttenStart.Location = new System.Drawing.Point(81, 61);
            this.farAttenStart.Name = "farAttenStart";
            this.farAttenStart.Size = new System.Drawing.Size(136, 20);
            this.farAttenStart.TabIndex = 9;
            // 
            // farAttenEnd
            // 
            this.farAttenEnd.DecimalPlaces = 5;
            this.farAttenEnd.Increment = new decimal(new int[] {
            5,
            0,
            0,
            196608});
            this.farAttenEnd.Location = new System.Drawing.Point(81, 87);
            this.farAttenEnd.Name = "farAttenEnd";
            this.farAttenEnd.Size = new System.Drawing.Size(136, 20);
            this.farAttenEnd.TabIndex = 10;
            // 
            // a3dOmniLightControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.farAttenEnd);
            this.Controls.Add(this.farAttenStart);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.groupBox1);
            this.Name = "a3dOmniLightControl";
            this.Size = new System.Drawing.Size(223, 172);
            this.Controls.SetChildIndex(this.groupBox1, 0);
            this.Controls.SetChildIndex(this.label3, 0);
            this.Controls.SetChildIndex(this.lightName, 0);
            this.Controls.SetChildIndex(this.lightColor, 0);
            this.Controls.SetChildIndex(this.label4, 0);
            this.Controls.SetChildIndex(this.farAttenStart, 0);
            this.Controls.SetChildIndex(this.farAttenEnd, 0);
            this.groupBox1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.farAttenStart)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.farAttenEnd)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private sceneEditor.Controls.VectorControl vectorControl1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown farAttenStart;
        private System.Windows.Forms.NumericUpDown farAttenEnd;
    }
}
