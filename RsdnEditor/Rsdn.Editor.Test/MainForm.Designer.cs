namespace Rsdn.Editor.Test
{
	partial class MainForm
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this._toolStripContainer = new System.Windows.Forms.ToolStripContainer();
            this._rsdnEdit = new Rsdn.Editor.Edit();
            this._propertyGrid = new System.Windows.Forms.PropertyGrid();
            this._mainMenuStrip = new System.Windows.Forms.MenuStrip();
            this._fileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._newMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._openMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._recentMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this._saveMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._saveAsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._menuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this._exitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._toolsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._showPropertiesMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this._mainToolStrip = new System.Windows.Forms.ToolStrip();
            this._newButton = new System.Windows.Forms.ToolStripButton();
            this._saveButton = new System.Windows.Forms.ToolStripButton();
            this._openButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this._wordWrapButton = new System.Windows.Forms.ToolStripButton();
            this._toggleInvisibleButton = new System.Windows.Forms.ToolStripButton();
            this._toggleAnimateButton = new System.Windows.Forms.ToolStripButton();
            this._doublleBufferingButton = new System.Windows.Forms.ToolStripButton();
            this._keyboardSettingsButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this._zoomComboBox = new System.Windows.Forms.ToolStripComboBox();
            this._testButton = new System.Windows.Forms.ToolStripButton();
            this._openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this._statusStrip = new System.Windows.Forms.StatusStrip();
            this._selectionToolLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this._selectionStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this._saveAsDialog = new System.Windows.Forms.SaveFileDialog();
            this._repaintTimer = new System.Windows.Forms.Timer(this.components);
            this._toolStripContainer.ContentPanel.SuspendLayout();
            this._toolStripContainer.TopToolStripPanel.SuspendLayout();
            this._toolStripContainer.SuspendLayout();
            this._mainMenuStrip.SuspendLayout();
            this._mainToolStrip.SuspendLayout();
            this._statusStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // _toolStripContainer
            // 
            // 
            // _toolStripContainer.ContentPanel
            // 
            this._toolStripContainer.ContentPanel.Controls.Add(this._rsdnEdit);
            this._toolStripContainer.ContentPanel.Controls.Add(this._propertyGrid);
            this._toolStripContainer.ContentPanel.Size = new System.Drawing.Size(699, 381);
            this._toolStripContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            this._toolStripContainer.Location = new System.Drawing.Point(0, 0);
            this._toolStripContainer.Name = "_toolStripContainer";
            this._toolStripContainer.Size = new System.Drawing.Size(699, 430);
            this._toolStripContainer.TabIndex = 1;
            this._toolStripContainer.Text = "toolStripContainer1";
            // 
            // _toolStripContainer.TopToolStripPanel
            // 
            this._toolStripContainer.TopToolStripPanel.Controls.Add(this._mainMenuStrip);
            this._toolStripContainer.TopToolStripPanel.Controls.Add(this._mainToolStrip);
            // 
            // _rsdnEdit
            // 
            this._rsdnEdit.Dock = System.Windows.Forms.DockStyle.Fill;
            this._rsdnEdit.DoubleBuffering = true;
            this._rsdnEdit.Location = new System.Drawing.Point(0, 0);
            this._rsdnEdit.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this._rsdnEdit.Name = "_rsdnEdit";
            this._rsdnEdit.ShowTextCaret = false;
            this._rsdnEdit.Size = new System.Drawing.Size(444, 381);
            this._rsdnEdit.Split = false;
            this._rsdnEdit.TabIndex = 0;
            this._rsdnEdit.WordWrap = false;
            this._rsdnEdit.Load += new System.EventHandler(this.rsdnEdit1_Load);
            this._rsdnEdit.DirtyChanged += new System.EventHandler(this._rsdnEdit_DirtyChanged);
            this._rsdnEdit.SelectionChanged += new System.EventHandler<Rsdn.Editor.SelectionChangedEventArgs>(this._rsdnEdit_SelectionChanged);
            this._rsdnEdit.BeforeChangeText += new System.EventHandler<Rsdn.Editor.BeforeChangeTexEventArgs>(this._rsdnEdit_BeforeChangeText);
            // 
            // _propertyGrid
            // 
            this._propertyGrid.Dock = System.Windows.Forms.DockStyle.Right;
            this._propertyGrid.Location = new System.Drawing.Point(444, 0);
            this._propertyGrid.Name = "_propertyGrid";
            this._propertyGrid.Size = new System.Drawing.Size(255, 381);
            this._propertyGrid.TabIndex = 1;
            this._propertyGrid.Visible = false;
            // 
            // _mainMenuStrip
            // 
            this._mainMenuStrip.Dock = System.Windows.Forms.DockStyle.None;
            this._mainMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._fileMenuItem,
            this._toolsMenuItem});
            this._mainMenuStrip.Location = new System.Drawing.Point(0, 0);
            this._mainMenuStrip.Name = "_mainMenuStrip";
            this._mainMenuStrip.Size = new System.Drawing.Size(699, 24);
            this._mainMenuStrip.TabIndex = 0;
            this._mainMenuStrip.Text = "menuStrip1";
            // 
            // _fileMenuItem
            // 
            this._fileMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._newMenuItem,
            this._openMenuItem,
            this._recentMenuItem,
            this.toolStripSeparator1,
            this._saveMenuItem,
            this._saveAsMenuItem,
            this._menuItem1,
            this._exitMenuItem});
            this._fileMenuItem.Name = "_fileMenuItem";
            this._fileMenuItem.Size = new System.Drawing.Size(35, 20);
            this._fileMenuItem.Text = "&File";
            // 
            // _newMenuItem
            // 
            this._newMenuItem.Image = global::Rsdn.Editor.Test.Properties.Resources.New_file_vs_24;
            this._newMenuItem.ImageTransparentColor = System.Drawing.Color.Fuchsia;
            this._newMenuItem.Name = "_newMenuItem";
            this._newMenuItem.Size = new System.Drawing.Size(136, 22);
            this._newMenuItem.Text = "New";
            this._newMenuItem.Click += new System.EventHandler(this._newButton_Click);
            // 
            // _openMenuItem
            // 
            this._openMenuItem.Name = "_openMenuItem";
            this._openMenuItem.Size = new System.Drawing.Size(136, 22);
            this._openMenuItem.Text = "&Open...";
            this._openMenuItem.Click += new System.EventHandler(this._openMenuItem_Click);
            // 
            // _recentMenuItem
            // 
            this._recentMenuItem.Name = "_recentMenuItem";
            this._recentMenuItem.Size = new System.Drawing.Size(136, 22);
            this._recentMenuItem.Text = "&Recent";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(133, 6);
            // 
            // _saveMenuItem
            // 
            this._saveMenuItem.Enabled = false;
            this._saveMenuItem.Image = global::Rsdn.Editor.Test.Properties.Resources.Save_file_vs_24;
            this._saveMenuItem.Name = "_saveMenuItem";
            this._saveMenuItem.Size = new System.Drawing.Size(136, 22);
            this._saveMenuItem.Text = "Save";
            this._saveMenuItem.Click += new System.EventHandler(this._saveButton_Click);
            // 
            // _saveAsMenuItem
            // 
            this._saveAsMenuItem.Name = "_saveAsMenuItem";
            this._saveAsMenuItem.Size = new System.Drawing.Size(136, 22);
            this._saveAsMenuItem.Text = "Save As...";
            this._saveAsMenuItem.Click += new System.EventHandler(this._saveAsMenuItem_Click);
            // 
            // _menuItem1
            // 
            this._menuItem1.Name = "_menuItem1";
            this._menuItem1.Size = new System.Drawing.Size(133, 6);
            // 
            // _exitMenuItem
            // 
            this._exitMenuItem.Name = "_exitMenuItem";
            this._exitMenuItem.Size = new System.Drawing.Size(136, 22);
            this._exitMenuItem.Text = "&Exit";
            this._exitMenuItem.Click += new System.EventHandler(this._exitMenuItem_Click);
            // 
            // _toolsMenuItem
            // 
            this._toolsMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._showPropertiesMenuItem});
            this._toolsMenuItem.Name = "_toolsMenuItem";
            this._toolsMenuItem.Size = new System.Drawing.Size(44, 20);
            this._toolsMenuItem.Text = "&Tools";
            // 
            // _showPropertiesMenuItem
            // 
            this._showPropertiesMenuItem.Name = "_showPropertiesMenuItem";
            this._showPropertiesMenuItem.Size = new System.Drawing.Size(163, 22);
            this._showPropertiesMenuItem.Text = "&Show properties";
            this._showPropertiesMenuItem.Click += new System.EventHandler(this._showPropertiesMenuItem_Click);
            // 
            // _mainToolStrip
            // 
            this._mainToolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this._mainToolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._newButton,
            this._saveButton,
            this._openButton,
            this.toolStripSeparator2,
            this._wordWrapButton,
            this._toggleInvisibleButton,
            this._toggleAnimateButton,
            this._doublleBufferingButton,
            this._keyboardSettingsButton,
            this.toolStripLabel1,
            this._zoomComboBox,
            this._testButton});
            this._mainToolStrip.Location = new System.Drawing.Point(3, 24);
            this._mainToolStrip.Name = "_mainToolStrip";
            this._mainToolStrip.Size = new System.Drawing.Size(344, 25);
            this._mainToolStrip.TabIndex = 1;
            this._mainToolStrip.Text = "toolStrip1";
            // 
            // _newButton
            // 
            this._newButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._newButton.Image = global::Rsdn.Editor.Test.Properties.Resources.New_file_vs_24;
            this._newButton.ImageTransparentColor = System.Drawing.Color.Fuchsia;
            this._newButton.Name = "_newButton";
            this._newButton.Size = new System.Drawing.Size(23, 22);
            this._newButton.Text = "toolStripButton1";
            this._newButton.Click += new System.EventHandler(this._newButton_Click);
            // 
            // _saveButton
            // 
            this._saveButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._saveButton.Enabled = false;
            this._saveButton.Image = global::Rsdn.Editor.Test.Properties.Resources.Save_file_vs_24;
            this._saveButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._saveButton.Name = "_saveButton";
            this._saveButton.Size = new System.Drawing.Size(23, 22);
            this._saveButton.Text = "Save";
            this._saveButton.Click += new System.EventHandler(this._saveButton_Click);
            // 
            // _openButton
            // 
            this._openButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._openButton.Image = ((System.Drawing.Image)(resources.GetObject("_openButton.Image")));
            this._openButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._openButton.Name = "_openButton";
            this._openButton.Size = new System.Drawing.Size(23, 22);
            this._openButton.Text = "Open file";
            this._openButton.Click += new System.EventHandler(this._openButton_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
            // 
            // _wordWrapButton
            // 
            this._wordWrapButton.Checked = global::Rsdn.Editor.Test.Properties.Settings.Default.WordWrap;
            this._wordWrapButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this._wordWrapButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._wordWrapButton.Image = ((System.Drawing.Image)(resources.GetObject("_wordWrapButton.Image")));
            this._wordWrapButton.ImageScaling = System.Windows.Forms.ToolStripItemImageScaling.None;
            this._wordWrapButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._wordWrapButton.Name = "_wordWrapButton";
            this._wordWrapButton.Size = new System.Drawing.Size(23, 22);
            this._wordWrapButton.Text = "Word wrap";
            this._wordWrapButton.Click += new System.EventHandler(this._wordWrapButton_Click);
            // 
            // _toggleInvisibleButton
            // 
            this._toggleInvisibleButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._toggleInvisibleButton.Image = ((System.Drawing.Image)(resources.GetObject("_toggleInvisibleButton.Image")));
            this._toggleInvisibleButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._toggleInvisibleButton.Name = "_toggleInvisibleButton";
            this._toggleInvisibleButton.Size = new System.Drawing.Size(23, 22);
            this._toggleInvisibleButton.Text = "Toggle Visual Space";
            this._toggleInvisibleButton.Click += new System.EventHandler(this._toggleInvisibleButton_Click);
            // 
            // _toggleAnimateButton
            // 
            this._toggleAnimateButton.Checked = true;
            this._toggleAnimateButton.CheckOnClick = true;
            this._toggleAnimateButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this._toggleAnimateButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._toggleAnimateButton.Image = ((System.Drawing.Image)(resources.GetObject("_toggleAnimateButton.Image")));
            this._toggleAnimateButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._toggleAnimateButton.Name = "_toggleAnimateButton";
            this._toggleAnimateButton.Size = new System.Drawing.Size(23, 22);
            this._toggleAnimateButton.Text = "Animate Images";
            this._toggleAnimateButton.Click += new System.EventHandler(this._toggleAbimateButton_Click);
            // 
            // _doublleBufferingButton
            // 
            this._doublleBufferingButton.Checked = true;
            this._doublleBufferingButton.CheckOnClick = true;
            this._doublleBufferingButton.CheckState = System.Windows.Forms.CheckState.Checked;
            this._doublleBufferingButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._doublleBufferingButton.Image = ((System.Drawing.Image)(resources.GetObject("_doublleBufferingButton.Image")));
            this._doublleBufferingButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._doublleBufferingButton.Name = "_doublleBufferingButton";
            this._doublleBufferingButton.Size = new System.Drawing.Size(23, 22);
            this._doublleBufferingButton.Text = "Doublle Buffering";
            this._doublleBufferingButton.Click += new System.EventHandler(this._doublleBufferingButton_Click);
            // 
            // _keyboardSettingsButton
            // 
            this._keyboardSettingsButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this._keyboardSettingsButton.Image = ((System.Drawing.Image)(resources.GetObject("_keyboardSettingsButton.Image")));
            this._keyboardSettingsButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._keyboardSettingsButton.Name = "_keyboardSettingsButton";
            this._keyboardSettingsButton.Size = new System.Drawing.Size(23, 22);
            this._keyboardSettingsButton.Text = "Keyboard settings";
            this._keyboardSettingsButton.Click += new System.EventHandler(this._keyboardSettingsButton_Click);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(33, 22);
            this.toolStripLabel1.Text = "Zoom";
            // 
            // _zoomComboBox
            // 
            this._zoomComboBox.DropDownHeight = 700;
            this._zoomComboBox.IntegralHeight = false;
            this._zoomComboBox.Name = "_zoomComboBox";
            this._zoomComboBox.Size = new System.Drawing.Size(75, 25);
            this._zoomComboBox.Text = "1.0";
            this._zoomComboBox.TextChanged += new System.EventHandler(this._zoomComboBox_TextChanged);
            // 
            // _testButton
            // 
            this._testButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this._testButton.Image = ((System.Drawing.Image)(resources.GetObject("_testButton.Image")));
            this._testButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this._testButton.Name = "_testButton";
            this._testButton.Size = new System.Drawing.Size(32, 22);
            this._testButton.Text = "Test";
            this._testButton.Click += new System.EventHandler(this._testButton_Click);
            // 
            // _openFileDialog
            // 
            this._openFileDialog.DefaultExt = "cs";
            this._openFileDialog.Filter = "All filse|*.*|C#|*.cs|C++|*.h*;*.cpp;*.cxx";
            this._openFileDialog.ShowReadOnly = true;
            this._openFileDialog.Title = "Open file";
            // 
            // _statusStrip
            // 
            this._statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this._selectionToolLabel,
            this._selectionStatusLabel});
            this._statusStrip.Location = new System.Drawing.Point(0, 430);
            this._statusStrip.Name = "_statusStrip";
            this._statusStrip.Padding = new System.Windows.Forms.Padding(1, 3, 14, 1);
            this._statusStrip.Size = new System.Drawing.Size(699, 22);
            this._statusStrip.TabIndex = 2;
            this._statusStrip.Text = "statusStrip1";
            // 
            // _selectionToolLabel
            // 
            this._selectionToolLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this._selectionToolLabel.Name = "_selectionToolLabel";
            this._selectionToolLabel.Size = new System.Drawing.Size(542, 14);
            this._selectionToolLabel.Spring = true;
            this._selectionToolLabel.Text = "_selectionToolLabel";
            this._selectionToolLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // _selectionStatusLabel
            // 
            this._selectionStatusLabel.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
            this._selectionStatusLabel.Name = "_selectionStatusLabel";
            this._selectionStatusLabel.Size = new System.Drawing.Size(111, 14);
            this._selectionStatusLabel.Text = "_selectionStatusLabel";
            // 
            // _saveAsDialog
            // 
            this._saveAsDialog.DefaultExt = "txt";
            // 
            // _repaintTimer
            // 
            this._repaintTimer.Interval = 1;
            this._repaintTimer.Tick += new System.EventHandler(this._repaintTimer_Tick);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(699, 452);
            this.Controls.Add(this._toolStripContainer);
            this.Controls.Add(this._statusStrip);
            this.KeyPreview = true;
            this.MainMenuStrip = this._mainMenuStrip;
            this.Name = "MainForm";
            this.Text = "Lines 1; ALL 0,00047961; DrawRow 0,00014187; MakeRanges 0,00000418; ExpandTabs 0," +
                "00000160; PaintRange 0,00007469; MeasureWidth 0,00000145; DrawText 0,00010575; ";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseUp);
            this.DoubleClick += new System.EventHandler(this.Form1_DoubleClick);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseDown);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Form1_KeyPress);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this._toolStripContainer.ContentPanel.ResumeLayout(false);
            this._toolStripContainer.TopToolStripPanel.ResumeLayout(false);
            this._toolStripContainer.TopToolStripPanel.PerformLayout();
            this._toolStripContainer.ResumeLayout(false);
            this._toolStripContainer.PerformLayout();
            this._mainMenuStrip.ResumeLayout(false);
            this._mainMenuStrip.PerformLayout();
            this._mainToolStrip.ResumeLayout(false);
            this._mainToolStrip.PerformLayout();
            this._statusStrip.ResumeLayout(false);
            this._statusStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStripContainer _toolStripContainer;
		private System.Windows.Forms.MenuStrip _mainMenuStrip;
		private System.Windows.Forms.ToolStripMenuItem _fileMenuItem;
		private System.Windows.Forms.ToolStripMenuItem _openMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem _exitMenuItem;
		private System.Windows.Forms.ToolStripMenuItem _toolsMenuItem;
		private System.Windows.Forms.ToolStripMenuItem _showPropertiesMenuItem;
		private System.Windows.Forms.PropertyGrid _propertyGrid;
		private System.Windows.Forms.ToolStripMenuItem _recentMenuItem;
		private System.Windows.Forms.OpenFileDialog _openFileDialog;
		private System.Windows.Forms.ToolStrip _mainToolStrip;
		private System.Windows.Forms.ToolStripButton _openButton;
		private System.Windows.Forms.ToolStripButton _wordWrapButton;
		private System.Windows.Forms.ToolStripButton _toggleInvisibleButton;
		private System.Windows.Forms.ToolStripButton _toggleAnimateButton;
		private System.Windows.Forms.ToolStripButton _doublleBufferingButton;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.StatusStrip _statusStrip;
		private System.Windows.Forms.ToolStripStatusLabel _selectionToolLabel;
		private System.Windows.Forms.ToolStripStatusLabel _selectionStatusLabel;
		private System.Windows.Forms.ToolStripButton _keyboardSettingsButton;
        private Rsdn.Editor.Edit _rsdnEdit;
		private System.Windows.Forms.ToolStripButton _newButton;
		private System.Windows.Forms.ToolStripButton _saveButton;
		private System.Windows.Forms.SaveFileDialog _saveAsDialog;
		private System.Windows.Forms.ToolStripMenuItem _newMenuItem;
		private System.Windows.Forms.ToolStripMenuItem _saveMenuItem;
		private System.Windows.Forms.ToolStripSeparator _menuItem1;
		private System.Windows.Forms.ToolStripMenuItem _saveAsMenuItem;
		private System.Windows.Forms.ToolStripLabel toolStripLabel1;
		private System.Windows.Forms.ToolStripComboBox _zoomComboBox;
		private System.Windows.Forms.ToolStripButton _testButton;
		private System.Windows.Forms.Timer _repaintTimer;
	}
}