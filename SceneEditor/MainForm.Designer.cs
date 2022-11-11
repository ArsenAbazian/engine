namespace sceneEditor {
	partial class MainForm {
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.menu = new System.Windows.Forms.MenuStrip();
            this.fileMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newSceneMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadSceneMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveSceneMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.separatorMenuItem = new System.Windows.Forms.ToolStripSeparator();
            this.exitMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.outputMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.timeLineMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dependencyTreeMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objectsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addModelMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addLandscapeMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addParticleSystemMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addRailSystemMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addLightMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addEffectMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addTextureMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addModelRefMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.removeModelRefToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.objListMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.landscapePropsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lightMapsManagerMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.generateLightMapsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateRadiosityMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.optionsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.settingsMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.contextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.propertiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.addModelDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveSceneDialog = new System.Windows.Forms.SaveFileDialog();
            this.toolbar = new System.Windows.Forms.ToolStrip();
            this.moveToolButton = new System.Windows.Forms.ToolStripButton();
            this.rotateToolButton = new System.Windows.Forms.ToolStripButton();
            this.scaleToolButton = new System.Windows.Forms.ToolStripButton();
            this.fillTypeBox = new System.Windows.Forms.ToolStripComboBox();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.lightMapGridButton = new System.Windows.Forms.ToolStripButton();
            this.splitControl = new System.Windows.Forms.SplitContainer();
            this.solutionControl = new sceneEditor.SolutionControl();
            this.outputControl1 = new sceneEditor.outputControl.OutputControl();
            this.timeLineControl = new sceneEditor.TimeLineEditor.TimeLineControl();
            this.viewControl = new sceneEditor.ViewControl();
            this.addEffectDialog = new System.Windows.Forms.OpenFileDialog();
            this.importModelDialog = new System.Windows.Forms.SaveFileDialog();
            this.addTextureDialog = new System.Windows.Forms.OpenFileDialog();
            this.scriptEditorMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menu.SuspendLayout();
            this.contextMenu.SuspendLayout();
            this.toolbar.SuspendLayout();
            this.splitControl.Panel1.SuspendLayout();
            this.splitControl.Panel2.SuspendLayout();
            this.splitControl.SuspendLayout();
            this.SuspendLayout();
            // 
            // menu
            // 
            this.menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileMenuItem,
            this.viewMenuItem,
            this.objectsMenuItem,
            this.objListMenuItem,
            this.toolsMenuItem,
            this.optionsMenuItem});
            this.menu.Location = new System.Drawing.Point(0, 0);
            this.menu.Name = "menu";
            this.menu.Size = new System.Drawing.Size(880, 24);
            this.menu.TabIndex = 0;
            this.menu.Text = "menuStrip1";
            // 
            // fileMenuItem
            // 
            this.fileMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newSceneMenuItem,
            this.loadSceneMenuItem,
            this.saveSceneMenuItem,
            this.separatorMenuItem,
            this.exitMenuItem});
            this.fileMenuItem.Name = "fileMenuItem";
            this.fileMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileMenuItem.Text = "&File";
            // 
            // newSceneMenuItem
            // 
            this.newSceneMenuItem.Name = "newSceneMenuItem";
            this.newSceneMenuItem.Size = new System.Drawing.Size(141, 22);
            this.newSceneMenuItem.Text = "&New Scene";
            this.newSceneMenuItem.Click += new System.EventHandler(this.newSceneMenuItem_Click);
            // 
            // loadSceneMenuItem
            // 
            this.loadSceneMenuItem.Name = "loadSceneMenuItem";
            this.loadSceneMenuItem.Size = new System.Drawing.Size(141, 22);
            this.loadSceneMenuItem.Text = "&Load Scene";
            this.loadSceneMenuItem.Click += new System.EventHandler(this.loadSceneMenuItem_Click);
            // 
            // saveSceneMenuItem
            // 
            this.saveSceneMenuItem.Name = "saveSceneMenuItem";
            this.saveSceneMenuItem.Size = new System.Drawing.Size(141, 22);
            this.saveSceneMenuItem.Text = "&Save Scene";
            this.saveSceneMenuItem.Click += new System.EventHandler(this.saveSceneMenuItem_Click);
            // 
            // separatorMenuItem
            // 
            this.separatorMenuItem.Name = "separatorMenuItem";
            this.separatorMenuItem.Size = new System.Drawing.Size(138, 6);
            // 
            // exitMenuItem
            // 
            this.exitMenuItem.Name = "exitMenuItem";
            this.exitMenuItem.Size = new System.Drawing.Size(141, 22);
            this.exitMenuItem.Text = "E&xit";
            // 
            // viewMenuItem
            // 
            this.viewMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.outputMenuItem,
            this.timeLineMenuItem,
            this.dependencyTreeMenuItem,
            this.scriptEditorMenuItem});
            this.viewMenuItem.Name = "viewMenuItem";
            this.viewMenuItem.Size = new System.Drawing.Size(41, 20);
            this.viewMenuItem.Text = "&View";
            // 
            // outputMenuItem
            // 
            this.outputMenuItem.Name = "outputMenuItem";
            this.outputMenuItem.Size = new System.Drawing.Size(167, 22);
            this.outputMenuItem.Text = "&Output";
            this.outputMenuItem.Click += new System.EventHandler(this.outputMenuItem_Click);
            // 
            // timeLineMenuItem
            // 
            this.timeLineMenuItem.Checked = true;
            this.timeLineMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
            this.timeLineMenuItem.Name = "timeLineMenuItem";
            this.timeLineMenuItem.Size = new System.Drawing.Size(167, 22);
            this.timeLineMenuItem.Text = "&TimeLine";
            this.timeLineMenuItem.Click += new System.EventHandler(this.timeLineMenuItem_Click);
            // 
            // dependencyTreeMenuItem
            // 
            this.dependencyTreeMenuItem.Name = "dependencyTreeMenuItem";
            this.dependencyTreeMenuItem.Size = new System.Drawing.Size(167, 22);
            this.dependencyTreeMenuItem.Text = "&DependencyTree";
            this.dependencyTreeMenuItem.Click += new System.EventHandler(this.dependencyTreeMenuItem_Click);
            // 
            // objectsMenuItem
            // 
            this.objectsMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addModelMenuItem,
            this.addLandscapeMenuItem,
            this.addParticleSystemMenuItem,
            this.addRailSystemMenuItem,
            this.addLightMenuItem,
            this.addEffectMenuItem,
            this.addTextureMenuItem,
            this.addModelRefMenuItem,
            this.removeModelRefToolStripMenuItem});
            this.objectsMenuItem.Name = "objectsMenuItem";
            this.objectsMenuItem.Size = new System.Drawing.Size(56, 20);
            this.objectsMenuItem.Text = "&Objects";
            // 
            // addModelMenuItem
            // 
            this.addModelMenuItem.Name = "addModelMenuItem";
            this.addModelMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addModelMenuItem.Text = "Add &Model";
            this.addModelMenuItem.Click += new System.EventHandler(this.addModelMenuItem_Click);
            // 
            // addLandscapeMenuItem
            // 
            this.addLandscapeMenuItem.Name = "addLandscapeMenuItem";
            this.addLandscapeMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addLandscapeMenuItem.Text = "Add &Landscape";
            this.addLandscapeMenuItem.Click += new System.EventHandler(this.addLandscapeMenuItem_Click);
            // 
            // addParticleSystemMenuItem
            // 
            this.addParticleSystemMenuItem.Name = "addParticleSystemMenuItem";
            this.addParticleSystemMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addParticleSystemMenuItem.Text = "Add &Particle System";
            // 
            // addRailSystemMenuItem
            // 
            this.addRailSystemMenuItem.Name = "addRailSystemMenuItem";
            this.addRailSystemMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addRailSystemMenuItem.Text = "Add &Rail System";
            this.addRailSystemMenuItem.Click += new System.EventHandler(this.addRailSystemMenuItem_Click);
            // 
            // addLightMenuItem
            // 
            this.addLightMenuItem.Name = "addLightMenuItem";
            this.addLightMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addLightMenuItem.Text = "Add &Light";
            // 
            // addEffectMenuItem
            // 
            this.addEffectMenuItem.Name = "addEffectMenuItem";
            this.addEffectMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addEffectMenuItem.Text = "Add &Effect";
            // 
            // addTextureMenuItem
            // 
            this.addTextureMenuItem.Name = "addTextureMenuItem";
            this.addTextureMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addTextureMenuItem.Text = "Add &Texture";
            this.addTextureMenuItem.Click += new System.EventHandler(this.addTextureMenuItem_Click);
            // 
            // addModelRefMenuItem
            // 
            this.addModelRefMenuItem.Name = "addModelRefMenuItem";
            this.addModelRefMenuItem.Size = new System.Drawing.Size(180, 22);
            this.addModelRefMenuItem.Text = "&Add Model Ref";
            this.addModelRefMenuItem.Click += new System.EventHandler(this.addModelRefMenuItem_Click);
            // 
            // removeModelRefToolStripMenuItem
            // 
            this.removeModelRefToolStripMenuItem.Name = "removeModelRefToolStripMenuItem";
            this.removeModelRefToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.removeModelRefToolStripMenuItem.Text = "&Remove Model Ref";
            this.removeModelRefToolStripMenuItem.Click += new System.EventHandler(this.removeModelRefToolStripMenuItem_Click);
            // 
            // objListMenuItem
            // 
            this.objListMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.landscapePropsMenuItem});
            this.objListMenuItem.Name = "objListMenuItem";
            this.objListMenuItem.Size = new System.Drawing.Size(70, 20);
            this.objListMenuItem.Text = "O&bject List";
            // 
            // landscapePropsMenuItem
            // 
            this.landscapePropsMenuItem.Name = "landscapePropsMenuItem";
            this.landscapePropsMenuItem.Size = new System.Drawing.Size(155, 22);
            this.landscapePropsMenuItem.Text = "Landscape List";
            this.landscapePropsMenuItem.Click += new System.EventHandler(this.landscapeListMenuItem_Click);
            // 
            // toolsMenuItem
            // 
            this.toolsMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lightMapsManagerMenuItem,
            this.toolStripMenuItem1,
            this.generateLightMapsMenuItem,
            this.generateRadiosityMenuItem});
            this.toolsMenuItem.Name = "toolsMenuItem";
            this.toolsMenuItem.Size = new System.Drawing.Size(44, 20);
            this.toolsMenuItem.Text = "&Tools";
            // 
            // lightMapsManagerMenuItem
            // 
            this.lightMapsManagerMenuItem.Name = "lightMapsManagerMenuItem";
            this.lightMapsManagerMenuItem.Size = new System.Drawing.Size(181, 22);
            this.lightMapsManagerMenuItem.Text = "LightMaps &Manager";
            this.lightMapsManagerMenuItem.Click += new System.EventHandler(this.lightMapsManagerMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(178, 6);
            // 
            // generateLightMapsMenuItem
            // 
            this.generateLightMapsMenuItem.Name = "generateLightMapsMenuItem";
            this.generateLightMapsMenuItem.Size = new System.Drawing.Size(181, 22);
            this.generateLightMapsMenuItem.Text = "Generate &LightMaps";
            // 
            // generateRadiosityMenuItem
            // 
            this.generateRadiosityMenuItem.Name = "generateRadiosityMenuItem";
            this.generateRadiosityMenuItem.Size = new System.Drawing.Size(181, 22);
            this.generateRadiosityMenuItem.Text = "Generate &Radiosity";
            // 
            // optionsMenuItem
            // 
            this.optionsMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.settingsMenuItem});
            this.optionsMenuItem.Name = "optionsMenuItem";
            this.optionsMenuItem.Size = new System.Drawing.Size(56, 20);
            this.optionsMenuItem.Text = "O&ptions";
            // 
            // settingsMenuItem
            // 
            this.settingsMenuItem.Name = "settingsMenuItem";
            this.settingsMenuItem.Size = new System.Drawing.Size(124, 22);
            this.settingsMenuItem.Text = "&Settings";
            this.settingsMenuItem.Click += new System.EventHandler(this.settingsMenuItem_Click);
            // 
            // contextMenu
            // 
            this.contextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.propertiesToolStripMenuItem});
            this.contextMenu.Name = "contextMenuStrip1";
            this.contextMenu.Size = new System.Drawing.Size(170, 26);
            // 
            // propertiesToolStripMenuItem
            // 
            this.propertiesToolStripMenuItem.Name = "propertiesToolStripMenuItem";
            this.propertiesToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
            this.propertiesToolStripMenuItem.Text = "Object Properties";
            // 
            // statusStrip
            // 
            this.statusStrip.Location = new System.Drawing.Point(0, 514);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(880, 22);
            this.statusStrip.TabIndex = 3;
            this.statusStrip.Text = "statusStrip1";
            // 
            // addModelDialog
            // 
            this.addModelDialog.FileName = "model";
            this.addModelDialog.Filter = "Model files (*.mdl) | *.mdl|Text Model files (*.bpm)| *.bpm|All files (*.*)|*.*";
            this.addModelDialog.Title = "Select model file";
            // 
            // saveSceneDialog
            // 
            this.saveSceneDialog.Filter = "Scene files (*.asc)|*.asc|All files (*.*)|*.*";
            // 
            // toolbar
            // 
            this.toolbar.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.toolbar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.moveToolButton,
            this.rotateToolButton,
            this.scaleToolButton,
            this.fillTypeBox,
            this.toolStripSeparator1,
            this.lightMapGridButton});
            this.toolbar.Location = new System.Drawing.Point(0, 24);
            this.toolbar.Name = "toolbar";
            this.toolbar.Size = new System.Drawing.Size(880, 31);
            this.toolbar.TabIndex = 4;
            this.toolbar.Text = "toolStrip1";
            // 
            // moveToolButton
            // 
            this.moveToolButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.moveToolButton.Image = global::sceneEditor.Properties.Resources.moveIcon;
            this.moveToolButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.moveToolButton.Name = "moveToolButton";
            this.moveToolButton.Size = new System.Drawing.Size(28, 28);
            this.moveToolButton.Text = "Move";
            this.moveToolButton.Click += new System.EventHandler(this.moveToolButton_Click);
            // 
            // rotateToolButton
            // 
            this.rotateToolButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.rotateToolButton.Image = global::sceneEditor.Properties.Resources.rotateIcon;
            this.rotateToolButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.rotateToolButton.Name = "rotateToolButton";
            this.rotateToolButton.Size = new System.Drawing.Size(28, 28);
            this.rotateToolButton.Text = "Rotate";
            // 
            // scaleToolButton
            // 
            this.scaleToolButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.scaleToolButton.Image = global::sceneEditor.Properties.Resources.scaleIcon;
            this.scaleToolButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.scaleToolButton.Name = "scaleToolButton";
            this.scaleToolButton.Size = new System.Drawing.Size(28, 28);
            this.scaleToolButton.Text = "Scale";
            // 
            // fillTypeBox
            // 
            this.fillTypeBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.fillTypeBox.Items.AddRange(new object[] {
            "Default",
            "Point",
            "Wireframe",
            "Solid"});
            this.fillTypeBox.Name = "fillTypeBox";
            this.fillTypeBox.Size = new System.Drawing.Size(121, 31);
            this.fillTypeBox.SelectedIndexChanged += new System.EventHandler(this.fillTypeBox_SelectedIndexChanged);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 31);
            // 
            // lightMapGridButton
            // 
            this.lightMapGridButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.lightMapGridButton.Image = ((System.Drawing.Image)(resources.GetObject("lightMapGridButton.Image")));
            this.lightMapGridButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.lightMapGridButton.Name = "lightMapGridButton";
            this.lightMapGridButton.Size = new System.Drawing.Size(28, 28);
            this.lightMapGridButton.Text = "Show LightMapGrid";
            this.lightMapGridButton.Click += new System.EventHandler(this.lightMapGridButton_Click);
            // 
            // splitControl
            // 
            this.splitControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitControl.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitControl.Location = new System.Drawing.Point(0, 55);
            this.splitControl.Name = "splitControl";
            // 
            // splitControl.Panel1
            // 
            this.splitControl.Panel1.Controls.Add(this.solutionControl);
            // 
            // splitControl.Panel2
            // 
            this.splitControl.Panel2.Controls.Add(this.outputControl1);
            this.splitControl.Panel2.Controls.Add(this.timeLineControl);
            this.splitControl.Panel2.Controls.Add(this.viewControl);
            this.splitControl.Size = new System.Drawing.Size(880, 459);
            this.splitControl.SplitterDistance = 198;
            this.splitControl.TabIndex = 5;
            // 
            // solutionControl
            // 
            this.solutionControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.solutionControl.Location = new System.Drawing.Point(0, 0);
            this.solutionControl.Name = "solutionControl";
            this.solutionControl.Size = new System.Drawing.Size(198, 459);
            this.solutionControl.TabIndex = 0;
            // 
            // outputControl1
            // 
            this.outputControl1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.outputControl1.Location = new System.Drawing.Point(0, 262);
            this.outputControl1.Name = "outputControl1";
            this.outputControl1.Size = new System.Drawing.Size(678, 139);
            this.outputControl1.TabIndex = 0;
            // 
            // timeLineControl
            // 
            this.timeLineControl.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.timeLineControl.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.timeLineControl.Location = new System.Drawing.Point(0, 401);
            this.timeLineControl.Name = "timeLineControl";
            this.timeLineControl.Size = new System.Drawing.Size(678, 58);
            this.timeLineControl.TabIndex = 2;
            this.timeLineControl.TimeChanged += new System.EventHandler(this.timeLineControl_TimeChanged);
            // 
            // viewControl
            // 
            this.viewControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.viewControl.Location = new System.Drawing.Point(0, 0);
            this.viewControl.Name = "viewControl";
            this.viewControl.Size = new System.Drawing.Size(678, 459);
            this.viewControl.TabIndex = 0;
            // 
            // addEffectDialog
            // 
            this.addEffectDialog.FileName = "effect.fx";
            this.addEffectDialog.Filter = "Effect files(*.fx)|*.fx|All files (*.*)|*.*";
            this.addEffectDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.addEffectDialog_FileOk);
            // 
            // importModelDialog
            // 
            this.importModelDialog.DefaultExt = "mdl";
            this.importModelDialog.Filter = "Model files (*.mdl) | *.mdl|All files (*.*)|*.*";
            this.importModelDialog.Title = "Please specify binary model filename and location";
            // 
            // addTextureDialog
            // 
            this.addTextureDialog.FileName = "texture";
            this.addTextureDialog.Filter = resources.GetString("addTextureDialog.Filter");
            this.addTextureDialog.FilterIndex = 6;
            this.addTextureDialog.Title = "Select texture file";
            // 
            // scriptEditorMenuItem
            // 
            this.scriptEditorMenuItem.Name = "scriptEditorMenuItem";
            this.scriptEditorMenuItem.Size = new System.Drawing.Size(167, 22);
            this.scriptEditorMenuItem.Text = "&ScriptEditor";
            this.scriptEditorMenuItem.Click += new System.EventHandler(this.scriptEditorMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(880, 536);
            this.Controls.Add(this.splitControl);
            this.Controls.Add(this.toolbar);
            this.Controls.Add(this.menu);
            this.Controls.Add(this.statusStrip);
            this.MainMenuStrip = this.menu;
            this.Name = "MainForm";
            this.Text = "a3dSceneEditor";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.menu.ResumeLayout(false);
            this.menu.PerformLayout();
            this.contextMenu.ResumeLayout(false);
            this.toolbar.ResumeLayout(false);
            this.toolbar.PerformLayout();
            this.splitControl.Panel1.ResumeLayout(false);
            this.splitControl.Panel2.ResumeLayout(false);
            this.splitControl.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menu;
		private System.Windows.Forms.ContextMenuStrip contextMenu;
		private System.Windows.Forms.StatusStrip statusStrip;
		private System.Windows.Forms.ToolStripMenuItem fileMenuItem;
		private System.Windows.Forms.ToolStripMenuItem newSceneMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadSceneMenuItem;
		private System.Windows.Forms.ToolStripMenuItem saveSceneMenuItem;
		private System.Windows.Forms.ToolStripSeparator separatorMenuItem;
		private System.Windows.Forms.ToolStripMenuItem exitMenuItem;
		private System.Windows.Forms.ToolStripMenuItem objectsMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addModelMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addLandscapeMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addParticleSystemMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addRailSystemMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addLightMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addEffectMenuItem;
		private System.Windows.Forms.ToolStripMenuItem addTextureMenuItem;
        private System.Windows.Forms.ToolStripMenuItem objListMenuItem;
		private System.Windows.Forms.ToolStripMenuItem optionsMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewMenuItem;
		private System.Windows.Forms.ToolStripMenuItem propertiesToolStripMenuItem;
		private System.Windows.Forms.OpenFileDialog addModelDialog;
		private System.Windows.Forms.ToolStripMenuItem settingsMenuItem;
        private System.Windows.Forms.SaveFileDialog saveSceneDialog;
        private System.Windows.Forms.ToolStripMenuItem addModelRefMenuItem;
        private System.Windows.Forms.ToolStripMenuItem removeModelRefToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem landscapePropsMenuItem;
        private System.Windows.Forms.ToolStrip toolbar;
        private System.Windows.Forms.SplitContainer splitControl;
        private SolutionControl solutionControl;
        private ViewControl viewControl;
        private System.Windows.Forms.ToolStripButton moveToolButton;
        private System.Windows.Forms.ToolStripButton rotateToolButton;
        private System.Windows.Forms.ToolStripButton scaleToolButton;
		private System.Windows.Forms.ToolStripComboBox fillTypeBox;
        private System.Windows.Forms.ToolStripMenuItem toolsMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lightMapsManagerMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem generateLightMapsMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateRadiosityMenuItem;
        private System.Windows.Forms.ToolStripButton lightMapGridButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.OpenFileDialog addEffectDialog;
        private System.Windows.Forms.ToolStripMenuItem outputMenuItem;
        private sceneEditor.outputControl.OutputControl outputControl1;
        private System.Windows.Forms.SaveFileDialog importModelDialog;
        private System.Windows.Forms.OpenFileDialog addTextureDialog;
        private System.Windows.Forms.ToolStripMenuItem timeLineMenuItem;
        private sceneEditor.TimeLineEditor.TimeLineControl timeLineControl;
        internal System.Windows.Forms.ToolStripMenuItem dependencyTreeMenuItem;
        private System.Windows.Forms.ToolStripMenuItem scriptEditorMenuItem;
	}
}

