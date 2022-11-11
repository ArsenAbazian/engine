// Код генерируемый дизайнером компонентов.

using Rsdn.Editor.ObjectModel;

namespace Rsdn.Editor
{
	partial class View
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this._brginDragTimer = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// _brginDragTimer
			// 
			this._brginDragTimer.Interval = 500;
			this._brginDragTimer.Tick += new System.EventHandler(this._brginDragTimer_Tick);
			// 
			// View
			// 
			this.AllowDrop = true;
			this.Cursor = System.Windows.Forms.Cursors.IBeam;
			this.QueryContinueDrag += new System.Windows.Forms.QueryContinueDragEventHandler(this.View_QueryContinueDrag);
			this.DragOver += new System.Windows.Forms.DragEventHandler(this.View_DragOver);
			this.DragDrop += new System.Windows.Forms.DragEventHandler(this.View_DragDrop);
			this.DragEnter += new System.Windows.Forms.DragEventHandler(this.View_DragEnter);
			this.GiveFeedback += new System.Windows.Forms.GiveFeedbackEventHandler(this.View_GiveFeedback);
			this.DragLeave += new System.EventHandler(this.View_DragLeave);
			this.ResumeLayout(false);

		}

		#endregion
	}
}
