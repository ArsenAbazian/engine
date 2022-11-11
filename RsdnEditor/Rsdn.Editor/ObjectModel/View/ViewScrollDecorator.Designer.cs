namespace Rsdn.Editor.ObjectModel
{
	partial class ViewScrollDecorator
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
			this._activeView = new Rsdn.Editor.View();
			this._hScrollBar = new System.Windows.Forms.HScrollBar();
			this._vScrollBar = new System.Windows.Forms.VScrollBar();
			this.SuspendLayout();
			// 
			// _activeView
			// 
			this._activeView.AllowDrop = true;
			this._activeView.Cursor = System.Windows.Forms.Cursors.IBeam;
			this._activeView.DoublleBuffering = false;
			this._activeView.HorizontalScrollStep = 10;
			this._activeView.Location = new System.Drawing.Point(10, 6);
			this._activeView.Name = "_activeView";
			this._activeView.Size = new System.Drawing.Size(349, 243);
			this._activeView.TabIndex = 2;
			this._activeView.Zoom = 1F;
			this._activeView.ViewChanged += new System.EventHandler(this._activeView_ViewChanged);
			// 
			// _hScrollBar
			// 
			this._hScrollBar.Location = new System.Drawing.Point(27, 279);
			this._hScrollBar.Maximum = 1000;
			this._hScrollBar.Name = "_hScrollBar";
			this._hScrollBar.Size = new System.Drawing.Size(80, 17);
			this._hScrollBar.TabIndex = 4;
			this._hScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this._hScrollBar_Scroll);
			// 
			// _vScrollBar
			// 
			this._vScrollBar.Location = new System.Drawing.Point(400, 28);
			this._vScrollBar.Name = "_vScrollBar";
			this._vScrollBar.Size = new System.Drawing.Size(17, 80);
			this._vScrollBar.TabIndex = 3;
			this._vScrollBar.Scroll += new System.Windows.Forms.ScrollEventHandler(this._vScrollBar_Scroll);
			// 
			// ViewScrollDecorator
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this._activeView);
			this.Controls.Add(this._hScrollBar);
			this.Controls.Add(this._vScrollBar);
			this.Name = "ViewScrollDecorator";
			this.Size = new System.Drawing.Size(427, 303);
			this.ResumeLayout(false);

		}

		#endregion

		private View _activeView;
		private System.Windows.Forms.HScrollBar _hScrollBar;
		private System.Windows.Forms.VScrollBar _vScrollBar;
	}
}
