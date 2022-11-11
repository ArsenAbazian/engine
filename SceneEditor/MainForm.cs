using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;
using System.Security.Permissions;
using sceneEditor.LightMapManager;
using sceneEditor.Lights;
using sceneEditor.ParticleSystem;
using sceneEditor.Dependency;
using sceneEditor.Configuration;
using System.IO;
using sceneEditor.TimeLineEditor;
using sceneEditor.a3dScript;

namespace sceneEditor {

    public partial class MainForm : Form {
        public MainForm() {
            InitializeComponent();
            LoadConfigutation();
            ViewControl.MainForm = this;
            SolutionControl.MainForm = this;
            TimeLineControl.MainForm = this;
            Application.Idle += new EventHandler(OnApplicationIdle);
        }

        Kernel_Mapper kernel = new Kernel_Mapper();
        camera_Mapper camera = new camera_Mapper();
        a3dGrid_Mapper grid = new a3dGrid_Mapper();
        a3dSceneMapper scene = new a3dSceneMapper();
        a3dLightMapGrid_Mapper lightMapGrid = new a3dLightMapGrid_Mapper();
        a3dBasisRender_Mapper basisRender = new a3dBasisRender_Mapper();

        SceneFilesInfoCollection recent;

        public Kernel_Mapper Kernel { get { return kernel; } }
        public camera_Mapper Camera { get { return camera; } }
        public a3dGrid_Mapper Grid { get { return grid; } }
        public a3dSceneMapper Scene { get { return scene; } }
        public a3dLightMapGrid_Mapper LightMapGrid { get { return lightMapGrid; } }
        public a3dBasisRender_Mapper BasisRender { get { return basisRender; } }

        public TimeLineControl TimeLineControl { get { return timeLineControl; } }

        public SceneFilesInfoCollection Recent {
            get {
                if (recent == null) {
                    recent = new SceneFilesInfoCollection();
                    recent.RecentCount = 9;
                }
                return recent;
            }
        }

        private void LoadConfigutation() {
            string path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\') + 1);
            Configurator.Default.LoadConfiguration(path + "config.xml");
        }

        private void SaveConfiguration() {
            string path = Application.ExecutablePath.Substring(0, Application.ExecutablePath.LastIndexOf('\\') + 1);
            Configurator.Default.LoadConfiguration(path + "config.xml");
        }
        
        internal void OnSettingsFormClose() {
            settingsForm = null;
            Configurator.Default.SerializeAllProperties("Kernel", Kernel);
            Configurator.Default.SerializeAllProperties("Grid", Grid);
            Configurator.Default.SerializeAllProperties("Camera", Camera);
            Configurator.Default.SerializeAllProperties("LightMapGrid", LightMapGrid);
            SaveConfiguration();
        }

        DependencyPropertyTreeForm depTreeForm;
        public DependencyPropertyTreeForm DependencyTreeForm {
            get {
                if (IsInvalidForm(depTreeForm)) {
                    depTreeForm = new DependencyPropertyTreeForm();
                    depTreeForm.MainForm = this;
                }
                return depTreeForm;
            }
        }

        SettingsForm settingsForm;
        public virtual SettingsForm SettingsForm {
            get {
                if (settingsForm == null)
                    settingsForm = new SettingsForm(this);
                return settingsForm;
            }
        }

        SceneSettingsForm sceneSettingsForm;
        public virtual SceneSettingsForm SceneSettingsForm {
            get {
                if (sceneSettingsForm == null)
                    sceneSettingsForm = new SceneSettingsForm(this);
                return sceneSettingsForm;
            }
        }

        LoadSceneForm loadSceneForm;
        public virtual LoadSceneForm LoadSceneForm {
            get {
				if(loadSceneForm == null || loadSceneForm.IsDisposed) {
                    loadSceneForm = new LoadSceneForm();
					loadSceneForm.MainForm = this;
				}
                return loadSceneForm;
            }
        }

        AddModelRefForm addModelRefForm;
        public virtual AddModelRefForm AddModelRefForm {
            get {
                if (addModelRefForm == null || addModelRefForm.IsDisposed) {
                    addModelRefForm = new AddModelRefForm();
                    addModelRefForm.MainForm = this;
                }
                return addModelRefForm;
            }
        }

        EffectListForm effectListForm;
        public virtual EffectListForm EffectListForm { 
            get {
                if (IsInvalidForm(effectListForm)) {
                    effectListForm = new EffectListForm();
                    effectListForm.MainForm = this;
                }
                return effectListForm;
            }
        }

        ModelRefCollectionForm modelRefCollForm;
        public virtual ModelRefCollectionForm ModelRefCollForm {
            get {
                if (modelRefCollForm == null || modelRefCollForm.IsDisposed)
                    modelRefCollForm = new ModelRefCollectionForm();
                return modelRefCollForm;
            }
        }

        AddLandscapeForm addLandForm;
        public virtual AddLandscapeForm AddLandForm {
            get {
                if (addLandForm == null || addLandForm.IsDisposed) {
                    addLandForm = new AddLandscapeForm();
                    addLandForm.MainForm = this;
                }
                return addLandForm;
            }
        }

        AddRailSystemForm addRailForm;
        public virtual AddRailSystemForm AddRailForm {
            get { 
                if(addRailForm == null || addRailForm.IsDisposed){
                    addRailForm = new AddRailSystemForm();
                    addRailForm.MainForm = this;
                }
                return addRailForm;
            }
        }

        LandscapePropsForm landPropsForm;
        public virtual LandscapePropsForm LandPropsForm {
            get { 
                if(landPropsForm == null || landPropsForm.IsDisposed) {
                    landPropsForm = new LandscapePropsForm();
                    landPropsForm.MainForm = this;
                }
                return landPropsForm;
            }
        }

        MoveForm moveForm;
        public MoveForm MoveForm { 
            get {
				if(moveForm == null || moveForm.IsDisposed) {
					moveForm = new MoveForm();
					moveForm.MainForm = this;
                }
				return moveForm;
            } 
        }

        InputHandler handler;
        public InputHandler Handler {
            get {
                if (handler == null) handler = new InputHandler(this);
                return handler;
            }
        }

        public virtual bool HasSelection {
            get { return false; }
        }

        public virtual Vector3_Mapper SelectionPivot {
            get {
                return new Vector3_Mapper(0.0f, 0.0f, 0.0f);
            }
        }

        public ViewControl ViewControl { get { return viewControl; } }

        public SolutionControl SolutionControl { get { return solutionControl; } }

        protected virtual void InitializeEngine() {
            if (!InitializeKernel()) {
                Close();
                return;
            }
            InitializeCamera();
            InitializeGrid();
            InitializeLightMapGrid();
            InitializeBasisRender();
            InitializeRecent();
        }

        protected virtual bool InitializeLightMapGrid() { 
            LightMapGrid.setKernel(Kernel);
            LightMapGrid.setCamera(Camera);
            Configurator.Default.DeserializeAllProperties("LightMapGrid", LightMapGrid);
            return true;
        }

        protected virtual bool InitializeBasisRender() {
            BasisRender.SetKernel(Kernel);
            BasisRender.SetCamera(Camera);
            return BasisRender.Initialize() == 0;
        }

        protected virtual bool InitializeKernel() {
            long res = 0;
            char[] path = "engine.cfg\0".ToCharArray();

            unsafe {
                fixed (char* cpath = path) {
                    res = kernel.Init(viewControl.Handle.ToInt32(), cpath);
                }
            }
            if (res != 0) {
                MessageBox.Show("Error: can't initialize kernel!", "sceneEditor", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return false;
            }

            Configurator.Default.DeserializeAllProperties("Kernel", Kernel);
            return true;
        }

        protected virtual void InitializeCamera() {
            camera.initialize(45.0f, 1.0f, 0.1f, 10000.0f);
            camera.ViewPortWidth = ClientRectangle.Width;
            camera.ViewPortHeight = ClientRectangle.Height;
            Configurator.Default.DeserializeAllProperties("Camera", camera);
            camera.UpdateCamera(kernel);
        }

        protected virtual void InitializeGrid() {
            grid.setKernel(kernel);
            grid.setCamera(camera);
            grid.initialize(20.0f, 1.0f, 10);
            grid.AxisColor = Color.Yellow;
            grid.LineColor = Color.LightGray;
            Configurator.Default.DeserializeAllProperties("Grid", grid);
        }

        protected override void OnLoad(EventArgs e) {
            base.OnLoad(e);
            InitializeEngine();
        }

        protected internal virtual void UpdateScene() {
            Camera.UpdateCamera(kernel);
            UpdateBasisRender();
            Scene.Update(Camera);
        }

        public OpenFileDialog AddModelDialog { get { return addModelDialog; } }
        public OpenFileDialog AddEffectDialog { get { return addEffectDialog; } }
        public OpenFileDialog AddTextureDialog { get { return addTextureDialog; } }

        public void AddTexture() {
            if (!IsSceneValid) return;
            if (AddTextureDialog.ShowDialog() == DialogResult.OK) {
                AddTexture(AddTextureDialog.FileName);
                SolutionControl.SceneView.RefreshSceneContent();
            }
        }
        
        public void AddModel() {
            if (!IsSceneValid) return;
            if (AddModelDialog.ShowDialog() == DialogResult.OK) {
                AddModel(AddModelDialog.FileName);
            }
        }

        public void AddEffect() {
            if (!IsSceneValid) return;
            AddEffectDialog.ShowDialog();
        }

        a3dLightForm lightForm;
        public a3dLightForm LightForm {
            get {
                if (IsInvalidForm(lightForm)) { 
                    lightForm = new a3dLightForm();
                    lightForm.MainForm = this;
                }
                return lightForm;    
            }
        }

        ParticleSystemForm particleSystemForm;
        public ParticleSystemForm ParticleSystemForm { 
            get { 
                if(IsInvalidForm(particleSystemForm)) {
                    particleSystemForm = new ParticleSystemForm();
                    particleSystemForm.MainForm = this;
                }
                return particleSystemForm;
            }
        }
        
        public bool IsSceneValid {
            get { return Scene.DataDirectory != null && Scene.DataDirectory != string.Empty; }
        }

        protected virtual void OnAddLight() {
            SolutionControl.SceneView.RefreshSceneContent();
            UpdateScene();
        }

        a3dOmniLightControl OmniLightControl { 
            get {
                if (LightForm == null) return null;
                return LightForm.LightControl.OmniLightControl;
            } 
        }

        a3dDirectionalLightControl DirLightControl { 
            get {
                if (LightForm == null) return null;
                return LightForm.LightControl.DirLightControl;
            }
        }

        protected virtual void AddOmniLight() {
            Scene.AddOmniLight(OmniLightControl.LightName, OmniLightControl.LightColor, OmniLightControl.FarAttenStart, OmniLightControl.FarAttenEnd, OmniLightControl.Position);
            OnAddLight();
        }

        protected virtual void AddDirectionalLight() {
            Scene.AddDirectionalLight(DirLightControl.LightName, DirLightControl.LightColor, DirLightControl.Direction);
            OnAddLight();
        }
        
        public void AddLight() {
            if (!IsSceneValid) return;
            if(LightForm.ShowDialog() == DialogResult.OK) {
                if (LightForm.LightControl.SelectedLightType == LightType.OmniLight)
                    AddOmniLight();
                else if (LightForm.LightControl.SelectedLightType == LightType.DirectionLight)
                    AddDirectionalLight();
            }
        }

        public particleSystemBase_Mapper CreateParticleSystem(string particleName) {
            switch (particleName) {
                case "Smoke": return new a3dSmokeParticleSystem_Mapper();
            }
            return new particleSystemBase_Mapper();
        }

        public a3dParticleSystemInfo_Mapper CreateParticleSystemInfo(string particleName) {
            a3dParticleSystemInfo_Mapper info = new a3dParticleSystemInfo_Mapper();
            info.ParticleName = ParticleSystemForm.ParticleInfo.ParticleName;
            switch(particleName) { 
                case "Smoke":
                    info.EffectFileName = "smoke.fx";
                    break;

            }
            return info;
        } 

        public void AddParticleSystem() {
            if (!IsSceneValid) return;
            if(ParticleSystemForm.ShowDialog() == DialogResult.OK) {
                particleSystemBase_Mapper particle = CreateParticleSystem(ParticleSystemForm.ParticleInfo.SelectedParticleTypeName);
                particle.PSysKernel = Kernel;
                particle.CurrCamera = Camera;
                particle.SetInt("ParticleCount", ParticleSystemForm.ParticleInfo.ParticleCount);
                particle.SetInt("MaxRenderParticleCount", ParticleSystemForm.ParticleInfo.MaxParticleCount);
                Scene.AddParticleSystem(ParticleSystemForm.ParticleInfo.ParticleName, particle);
                SolutionControl.SceneView.RefreshSceneContent();
            }
        }
        
        private void addModelMenuItem_Click(object sender, EventArgs e) {
            if (!IsSceneValid) return;
            AddModel();
        }

        protected virtual bool IsTextModelFile(string fileName) {
            if (fileName.EndsWith(".bpm")) return true;
            StreamReader r = new StreamReader(fileName);
            if (r.ReadLine() == "a3dModel") return true;
            r.Close();
            return false;
        }

        protected virtual void AddTexture(string fileName) {
            if (!Scene.AddTexture(fileName)) {
                MessageBox.Show(this, "Error: can't add texture '" + fileName + "' to scene.");
                return;
            }
            MessageBox.Show(this, "Texture successfully added");
        }
        
        protected virtual void AddModel(string fileName) {
            string finalFileName = string.Empty;
            if (!fileName.Contains(Scene.ModelDirectoryFull)) {
                MessageBox.Show(this, "Error: model file must be located in 'models' directory.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            else if (Scene.DataDirectory == "") {
                MessageBox.Show(this, "Error: you must specify scene directories. Use 'New Scene' Menu item", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            if (IsTextModelFile(fileName)) {
                if (MessageBox.Show(this, "You trying to add text model file to the scene. This model must be converted to binary model. Convert?", "Add Text Model File", MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes) {
                    if (!ShowImportModelDialog(fileName, out finalFileName)) {
                        MessageBox.Show(this, "Error: can't add model from file '" + fileName + "'. Error while converting text model file to binary.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                        return;
                    }
                }
                else 
                    return;
            }
            bool res;
            if (finalFileName != string.Empty)
                res = Scene.AddModel(finalFileName);
            else
                res = Scene.AddModel(fileName);
            if (!res) {
                MessageBox.Show(this, "Error: can't add model from file '" + fileName + "'.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }
            MessageBox.Show(this, "Model successfully added", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
        }

        protected virtual string GetImportModelDestinationFullPathName(string fileName) {
            FileInfo fi = new FileInfo(fileName);
            string res = fi.Name;
            if(fi.Name.EndsWith(".bpm")) 
                res = fi.Name.Substring(0, fi.Name.Length - 4);

            if (fi.Directory.FullName.Contains(Scene.ModelDirectoryFull))
                return fi.Directory.FullName + "\\" + res + ".mdl";
            return Scene.ModelDirectoryFull + "\\" + res + ".mdl";
        }

        protected virtual bool ShowImportModelDialog(string fileName, out string finalFileName) {
            finalFileName = string.Empty;
            importModelDialog.FileName = GetImportModelDestinationFullPathName(fileName);
            if (importModelDialog.ShowDialog() == DialogResult.OK) {
                if (!Scene.ImportModel(fileName, importModelDialog.FileName)) {
                    MessageBox.Show(this, "Error: can't convert text model file '" + fileName + "' to binary model file name.", "TextModelFile conversion", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return false;
                }
                finalFileName = importModelDialog.FileName;
                return true;
            }
            return false;
        }

        private void settingsMenuItem_Click(object sender, EventArgs e) {
            SettingsForm.Show();
        }

        private void newSceneMenuItem_Click(object sender, EventArgs e) {
            if (ProcessPrevScene("New Scene")) {
                NewScene();
                return;
            }
        }

        protected virtual bool ProcessPrevScene(string reason) {
            DialogResult res = MessageBox.Show("Do you wan't to save current scene?", reason, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);
            if (res == DialogResult.Cancel) return false;
            if (res == DialogResult.No || SaveScene()) return true;
            return false;
        }

        protected virtual bool SaveScene() {
            if (saveSceneDialog.ShowDialog() == DialogResult.OK) {
                bool res = Scene.SaveScene(saveSceneDialog.FileName);
                if (!res) {
                    MessageBox.Show("Error: can't save scene.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
                else {
                    MessageBox.Show("Scene saved to '" + saveSceneDialog.FileName + "'.", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                    AddRecent(saveSceneDialog.FileName, Scene.DataDirectory);
                }
                return res;
            }
            return false;
        }

        protected virtual bool LoadScene(string fileName, string dataDir) {
            Scene.DesignMode = true;
            Scene.SetKernel(Kernel);
            bool res = Scene.LoadScene(fileName, dataDir);
            if (!res) {
                MessageBox.Show("Error: can't load scene.", Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            else {
                MessageBox.Show(this, "Scene successfully loaded!", Text, MessageBoxButtons.OK, MessageBoxIcon.Information);
                AddRecent(Scene.FullPathName, Scene.DataDirectory);
            }
            return res;
        }

        protected virtual bool LoadScene() {
            if (LoadSceneForm.ShowDialog() == DialogResult.OK) {
                return LoadScene(LoadSceneForm.SceneFileName, LoadSceneForm.DataDirectoryName);
            }
            return false;
        }

        protected virtual void NewScene() {
            if (SceneSettingsForm.ShowDialog() == DialogResult.OK) {
                Scene.Clear();
                Scene.DesignMode = true;
                Scene.SetKernel(this.Kernel);
                Scene.MakeNewScene(SceneSettingsForm.SceneDataDirectory,
                                       SceneSettingsForm.TextureDirectory,
                                       SceneSettingsForm.EffectDirectory,
                                       SceneSettingsForm.ModelDirectory,
                                       SceneSettingsForm.LandDirectory,
                                       SceneSettingsForm.ParticleDirectory,
                                       SceneSettingsForm.RailDirectory,
                                       SceneSettingsForm.FontDirectory,
                                       SceneSettingsForm.SoundDirectory);
            }
        }

        private void loadSceneMenuItem_Click(object sender, EventArgs e) {
            if (ProcessPrevScene("Load Scene")) {
                LoadScene();
            }
        }

        private void saveSceneMenuItem_Click(object sender, EventArgs e) {
            SaveScene();
        }

        protected virtual void InitializeRecent() {
            Recent.DeSerialize("BoogiePerets\\SceneEditor v1.0");
            InitializeRecentMenu();

            return;
        }

        protected virtual void InitializeRecentMenu() {
            int itemIndex = 1;

            foreach (SceneFilesInfo info in Recent) {
                if (info.SceneFile == null || info.SceneFile == string.Empty || info.DataDir == null || info.DataDir == string.Empty) continue;
                ToolStripMenuItem item = new ToolStripMenuItem("&" + itemIndex.ToString() + " " + info.SceneFile + " + " + info.DataDir, null, new EventHandler(OnRecentClick));
                item.Tag = info;
                item.MouseDown += new MouseEventHandler(OnRecentRightClick);
                fileMenuItem.DropDownItems.Insert(fileMenuItem.DropDownItems.IndexOf(exitMenuItem), item);
                if (itemIndex < 9)
                    itemIndex++;
            }

            if (itemIndex > 1) {
                fileMenuItem.DropDownItems.Insert(fileMenuItem.DropDownItems.IndexOf(exitMenuItem), new ToolStripSeparator());
            }
        }

        protected virtual void ClearRecentMenu() {
            while (true) {
                ToolStripItem item = fileMenuItem.DropDownItems[fileMenuItem.DropDownItems.IndexOf(separatorMenuItem) + 1];
                if (item == exitMenuItem) break;
                fileMenuItem.DropDownItems.Remove(item);
            }
        }

        protected virtual void SaveRecent() {
            Recent.Serialize("BoogiePerets\\SceneEditor v1.0");
            return;
        }

        protected virtual void OnRecentRightClick(object sender, MouseEventArgs e) {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            if (item == null || e.Button != MouseButtons.Right)
                return;
            if (MessageBox.Show(this, "Do you really want to remove this scene from recent list?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.No)
                return;

            SceneFilesInfo info = item.Tag as SceneFilesInfo;
            if (info == null)
                return;

            Recent.Remove(info);
            ClearRecentMenu();
            InitializeRecentMenu();
            SaveRecent();
        }

        protected virtual void OnRecentClick(object sender, EventArgs e) {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            if (item == null) return;

            LoadRecentScene(item.Tag as SceneFilesInfo);
        }

        protected virtual bool LoadRecentScene(SceneFilesInfo info) {
            if (ProcessPrevScene("Load Scene")) {
                return LoadScene(info.SceneFile, info.DataDir);
            }
            return false;
        }

        protected virtual void AddRecent(string sceneFile, string dataDir) {
            SceneFilesInfo info = new SceneFilesInfo(sceneFile, dataDir);

            if (Recent.Contains(info))
                return;

            Recent.Insert(0, new SceneFilesInfo(sceneFile, dataDir));

            ClearRecentMenu();
            InitializeRecentMenu();
            SaveRecent();
        }

        private void addModelRefMenuItem_Click(object sender, EventArgs e) {
            ShowAddModelRefForm();
        }

        protected virtual void ShowAddModelRefForm() {
            AddModelRefForm.MainForm = this;
            AddModelRefForm.Show();
        }

        private void removeModelRefToolStripMenuItem_Click(object sender, EventArgs e) {
            ModelRefCollForm.MainForm = this;
            ModelRefCollForm.Show();
        }

        private void addLandscapeMenuItem_Click(object sender, EventArgs e) {
            AddLandForm.MainForm = this;
            AddLandForm.ShowDialog();
        }

        LandPassInfoForm landPassInfoForm;
        public LandPassInfoForm LandPassInfoForm {
            get {
                if (landPassInfoForm == null || landPassInfoForm.IsDisposed)
                    landPassInfoForm = new LandPassInfoForm();
                return landPassInfoForm;
            }
        }

        protected virtual bool IsInvalidForm(Form frm) {
            return frm == null || frm.IsDisposed;
        }

        LandscapeListForm landListForm;
        public LandscapeListForm LandListForm {
            get {
                if (IsInvalidForm(landListForm))
                    landListForm = new LandscapeListForm();
                return landListForm;
            }
        }

        LightMapInfoManagerForm lightMapManagerForm;
        public LightMapInfoManagerForm LightMapInfoManagerForm { 
            get {
                if (IsInvalidForm(lightMapManagerForm)) {
                    lightMapManagerForm = new LightMapInfoManagerForm();
                    lightMapManagerForm.MainForm = this;
                }
                return lightMapManagerForm;
            }
        }

        private void landscapeListMenuItem_Click(object sender, EventArgs e) {
            LandListForm.MainForm = this;
            LandListForm.Show();
        }

        public void UpdateForm() { 
            Scene.Update(Camera);
            ViewControl.Invalidate();
        }

        private void moveToolButton_Click(object sender, EventArgs e) {
            if(SolutionControl.SceneView.SelectedNode == null) return;
            MoveForm.ObjectWithBasis = SolutionControl.SceneView.SelectedNode.Tag as IObjectWithBasis;
			MoveForm.Show();
        }

		private void fillTypeBox_SelectedIndexChanged(object sender, EventArgs e) {
			if(fillTypeBox.SelectedIndex == 0)
				Kernel.FillType = RenderFillType.Default;
			else if(fillTypeBox.SelectedIndex == 1)
				Kernel.FillType = RenderFillType.Point;
			else if(fillTypeBox.SelectedIndex == 2)
				Kernel.FillType = RenderFillType.Wireframe;
			else if(fillTypeBox.SelectedIndex == 3)
				Kernel.FillType = RenderFillType.Solid;
            UpdateScene();
        }

        private void addRailSystemMenuItem_Click(object sender, EventArgs e) {
            AddRailForm.ShowDialog();
        }

        private void lightMapsManagerMenuItem_Click(object sender, EventArgs e) {
            LightMapInfoManagerForm.Show();
        }

        public bool ShowLightMapGrid { get { return lightMapGridButton.Checked; } }
        private void lightMapGridButton_Click(object sender, EventArgs e) {
            lightMapGridButton.Checked = !lightMapGridButton.Checked;
            UpdateLightMapGrid();
        }

        public virtual void UpdateLightMapGrid() {
            if (ShowLightMapGrid && SolutionControl.SceneView.SelectedNode != null) {
                ILightMapCreatorProvider prov = SolutionControl.SceneView.SelectedNode.Tag as ILightMapCreatorProvider;
                if (prov != null) {
                    LightMapGrid.setLightMapCreator(prov.LightMapCreator);
                }
            }
            UpdateForm();
        }

        public virtual void UpdateBasisRender() {
            if (SolutionControl.SceneView.SelectedNode != null) {
                a3dModelRef_Mapper modelRef = SolutionControl.SceneView.SelectedNode.Tag as a3dModelRef_Mapper;
                a3dLandscapeDesc_Mapper land = SolutionControl.SceneView.SelectedNode.Tag as a3dLandscapeDesc_Mapper;
                if (modelRef != null)
                    BasisRender.Basis = modelRef.Basis;
                else if (land != null)
                    BasisRender.Basis = land.Basis;
            }
            UpdateForm();
        }

        private void addEffectDialog_FileOk(object sender, CancelEventArgs e) {
            Scene.AddEffect(AddEffectDialog.FileName);
            SolutionControl.SceneView.RefreshSceneContent();
        }

        private void outputMenuItem_Click(object sender, EventArgs e) {
            if (outputMenuItem.CheckState == CheckState.Checked)
                outputMenuItem.CheckState = CheckState.Unchecked;
            else
                outputMenuItem.CheckState = CheckState.Checked;
            outputControl1.Visible = outputMenuItem.CheckState != CheckState.Unchecked;
        }

        DependencyForm dependencyForm;
        public DependencyForm DependencyForm { 
            get {
                if (IsInvalidForm(dependencyForm)) {
                    dependencyForm = new DependencyForm();
                    dependencyForm.MainForm = this;
                }
                return dependencyForm;
            }
        }

        public void ShowDependencyPropertyForm(IDependencyPropertyOwner propertyOwner) {
            DependencyForm.PropertyControl.PropertyOwner = propertyOwner;
            DependencyForm.Show();
        }

        private void addTextureMenuItem_Click(object sender, EventArgs e) {
            AddTexture();
        }

        private void timeLineMenuItem_Click(object sender, EventArgs e) {
            if (timeLineMenuItem.Checked) {
                timeLineMenuItem.Checked = false;
                timeLineControl.Visible = false;
            }
            else {
                timeLineMenuItem.Checked = true;
                timeLineControl.Visible = true;
            }
        }

        private void timeLineControl_TimeChanged(object sender, EventArgs e) {
            Scene.TimeProvider.Time = timeLineControl.Time / 1000.0f;
            ViewControl.Invalidate();
        }

        protected virtual void OnApplicationIdle(object sender, EventArgs e) {
            //Invalidate();
        }

        private void dependencyTreeMenuItem_Click(object sender, EventArgs e) {
            dependencyTreeMenuItem.Checked = !DependencyTreeForm.Visible;
            DependencyTreeForm.RootNode = Scene;
            DependencyTreeForm.Visible = !DependencyTreeForm.Visible;
        }

        private void scriptEditorMenuItem_Click(object sender, EventArgs e) {
            using(ScriptForm form = new ScriptForm()) {
                form.MainForm = this;
                form.ShowDialog();
            }
        }
    }
}