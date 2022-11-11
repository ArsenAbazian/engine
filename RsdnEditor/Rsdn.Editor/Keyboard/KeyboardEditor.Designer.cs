namespace Rsdn.Editor.Keyboard
{
	partial class KeyboardEditor
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this._dataGridView = new System.Windows.Forms.DataGridView();
			((System.ComponentModel.ISupportInitialize)(this._dataGridView)).BeginInit();
			this.SuspendLayout();
			// 
			// _dataGridView
			// 
			this._dataGridView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
									| System.Windows.Forms.AnchorStyles.Left)
									| System.Windows.Forms.AnchorStyles.Right)));
			this._dataGridView.Location = new System.Drawing.Point(3, 3);
			this._dataGridView.Name = "_dataGridView";
			this._dataGridView.Size = new System.Drawing.Size(594, 361);
			this._dataGridView.TabIndex = 0;
			this._dataGridView.Text = "dataGridView1";
			this._dataGridView.DataError += new System.Windows.Forms.DataGridViewDataErrorEventHandler(this._dataGridView_DataError);
			// 
			// KeyboardEditor
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this._dataGridView);
			this.Name = "KeyboardEditor";
			this.Size = new System.Drawing.Size(601, 394);
			this.Load += new System.EventHandler(this.KeyboardEditor_Load);
			((System.ComponentModel.ISupportInitialize)(this._dataGridView)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.DataGridView _dataGridView;
	}
}
