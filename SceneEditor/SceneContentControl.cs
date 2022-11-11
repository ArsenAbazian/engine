using System.Windows.Forms;
using classMapper;
using System.ComponentModel;

namespace sceneEditor {
    public class SceneContentControl : TreeView {
		a3dSceneMapper scene;
        private ImageList imageList1;
        private IContainer components;
		SceneContentOptions optionsContent;

        public SceneContentControl() {
			optionsContent = new SceneContentOptions(this);
            InitializeComponent();
            this.ImageList = imageList1;
        }

        [DefaultValue(null)]
        public a3dSceneMapper Scene {
            get { return scene; }
            set {
                if (Scene == value) return;
                scene = value;
                OnSceneChanged();
            }
        }

        protected virtual SceneContentOptions CreateContentOptions() { return new SceneContentOptions(this); }
        public virtual bool CanSetLightMapCheck(TreeNode node) {
            if (node == Nodes[0]) return false;
            a3dLightBase_Mapper lightMap = node.Tag as a3dLightBase_Mapper;
            if (lightMap != null) return true;
            a3dModelRef_Mapper modelRef = node.Tag as a3dModelRef_Mapper;
            if (modelRef != null) return true;
            a3dLandscapeDesc_Mapper land = node.Tag as a3dLandscapeDesc_Mapper;
            if (land != null) return true;
            a3dRailSystemDesc_Mapper rail = node.Tag as a3dRailSystemDesc_Mapper;
            if (rail != null) return true;
            return false;
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content), Browsable(true), EditorBrowsable(EditorBrowsableState.Advanced)]
		public SceneContentOptions OptionsContent { 
            get {
                if (optionsContent == null) optionsContent = CreateContentOptions();
                return optionsContent; 
            }
            set { optionsContent = value; }
        }

        protected virtual void OnSceneChanged() {
            ClearSceneContent();
			InitializeSceneContent();
        }

        protected virtual void ClearSceneContent() {
            Nodes.Clear();
        }

        public string SceneString { get { return "Scene"; } }
        public string TexturesString { get { return "Textures"; } }
        public string EffectsString { get { return "Effects";  } }
        public string ModelsString { get { return "Models"; } }
        public string LandscapesString { get { return "Landscapes"; } }
        public string ParticleSystemsString { get { return "Particle Systems"; } }
        public string RailwaySystemsString { get { return "Railway Systems"; } }
        public string LightsString { get { return "Lights"; } }
        public string ModelRefString { get { return "ModelRef"; } }
        public string FontsString { get { return "Fonts"; } }
        public string SoundsString { get { return "Sounds"; } }
        public string MaterialsString { get { return "Materials"; } }

        public bool IsTexturesNode { 
            get { 
                if(SelectedNode == null) return false;
                return SelectedNode.Tag is a3dTextureDesc_Mapper || SelectedNode.Text == TexturesString;
            } 
        }

        public bool IsEffectsNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dEffectDesc_Mapper || SelectedNode.Text == EffectsString;
            }
        }

        public bool IsModelsNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dModelDesc_Mapper || SelectedNode.Text == ModelsString;
            }
        }

        public bool IsLandscapesNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dLandscapeDesc_Mapper || SelectedNode.Text == LandscapesString;
            }
        }

        public bool IsParticlesNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dParticleSystemDesc_Mapper || SelectedNode.Text == ParticleSystemsString;
            }
        }

        public bool IsRailsNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dParticleSystemDesc_Mapper || SelectedNode.Text == RailwaySystemsString;
            }
        }

        public bool IsLightsNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dLightBase_Mapper || SelectedNode.Text == LightsString;
            }
        }

        public bool IsModelRefsNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dModelRef_Mapper || SelectedNode.Text == ModelRefString;
            }
        }

        public bool IsDetailNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dDetail_Mapper;
            }
        }
        
        public bool IsDetailRefNode { 
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dDetailRefMapper;
            }
        }

        public bool IsFontsNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dFontDesc_Mapper || SelectedNode.Text == FontsString;
            }
        }

        public bool IsSoundsNode {
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dSoundDesc_Mapper || SelectedNode.Text == SoundsString;
            }
        }

        public bool IsMaterialNode { 
            get {
                if (SelectedNode == null) return false;
                return SelectedNode.Tag is a3dShaderMaterialMapper || SelectedNode.Text == MaterialsString;
            }
        }

        public a3dDetailRefMapper SelectedDetailRef { get { return IsDetailRefNode ? SelectedNode.Tag as a3dDetailRefMapper : null; } }
        public a3dEffectDesc_Mapper SelectedEffectDesc { get { return IsEffectsNode ? SelectedNode.Tag as a3dEffectDesc_Mapper : null; } }
        public a3dShaderMaterialMapper SelectedMaterial { get { return IsMaterialNode ? SelectedNode.Tag as a3dShaderMaterialMapper : null; } }
        public a3dParticleSystemDesc_Mapper SelectedParticle { get { return IsParticlesNode ? SelectedNode.Tag as a3dParticleSystemDesc_Mapper : null; } }

        protected virtual void InitializeSceneContent() {

            TreeNode texNode = null;
            TreeNode effNode = null;
            TreeNode modNode = null;
            TreeNode landNode = null;
            TreeNode partNode = null;
            TreeNode railNode = null;
            TreeNode lightNode = null;
            TreeNode fontNode = null;
            TreeNode soundNode = null;
            TreeNode refNode = null;
            TreeNode sceneNode = null;

            sceneNode = new TreeNode(SceneString);

            if (OptionsContent.ViewTextures) {
                texNode = new TreeNode(TexturesString);
                texNode.ImageIndex = (int)SceneContentControlIcons.Textures;
                texNode.SelectedImageIndex = (int)SceneContentControlIcons.Textures;
                sceneNode.Nodes.Add(texNode);
                InitializeTextures(texNode);
            }

			if(OptionsContent.ViewEffects) {
                effNode = new TreeNode(EffectsString);
                effNode.ImageIndex = (int)SceneContentControlIcons.Effects;
                effNode.SelectedImageIndex = (int)SceneContentControlIcons.Effects;
                sceneNode.Nodes.Add(effNode);
                InitializeEffects(effNode);
            }

			if(OptionsContent.ViewModels) {
                modNode = new TreeNode(ModelsString);
                modNode.ImageIndex = (int)SceneContentControlIcons.Models;
                modNode.SelectedImageIndex = (int)SceneContentControlIcons.Models;
                sceneNode.Nodes.Add(modNode);
                InitializeModels(modNode);
            }

			if(OptionsContent.ViewLands) {
                landNode = new TreeNode(LandscapesString);
                landNode.ImageIndex = (int)SceneContentControlIcons.Lands;
                landNode.SelectedImageIndex = (int)SceneContentControlIcons.Lands;
                sceneNode.Nodes.Add(landNode);
                InitializeLandscapes(landNode);
            }

			if(OptionsContent.ViewParticles) {
                partNode = new TreeNode(ParticleSystemsString);
                partNode.ImageIndex = (int)SceneContentControlIcons.Particles;
                partNode.SelectedImageIndex = (int)SceneContentControlIcons.Particles;
                sceneNode.Nodes.Add(partNode);
                InitializeParticles(partNode);
            }

			if(OptionsContent.ViewRails) {
                railNode = new TreeNode(RailwaySystemsString);
                railNode.ImageIndex = (int)SceneContentControlIcons.Rails;
                railNode.SelectedImageIndex = (int)SceneContentControlIcons.Rails;
                sceneNode.Nodes.Add(railNode);
                InitializeRails(railNode);
            }

			if(OptionsContent.ViewLights) {
                lightNode = new TreeNode(LightsString);
                lightNode.ImageIndex = (int)SceneContentControlIcons.Lights;
                lightNode.SelectedImageIndex = (int)SceneContentControlIcons.Lights;
                sceneNode.Nodes.Add(lightNode);
                InitializeLights(lightNode);
            }

            if (OptionsContent.ViewModelRefs) {
                refNode = new TreeNode(ModelRefString);
                refNode.ImageIndex = (int)SceneContentControlIcons.ModelRefs;
                refNode.SelectedImageIndex = (int)SceneContentControlIcons.ModelRefs;
                sceneNode.Nodes.Add(refNode);
                InitializeModelRefs(refNode);
            }

            if (OptionsContent.ViewFonts) {
                fontNode = new TreeNode(FontsString);
                fontNode.ImageIndex = (int)SceneContentControlIcons.Default;
                fontNode.SelectedImageIndex = (int)SceneContentControlIcons.Default;
                sceneNode.Nodes.Add(fontNode);
                InitializeFonts(fontNode);
            }

            if (OptionsContent.ViewSounds) {
                soundNode = new TreeNode(SoundsString);
                soundNode.ImageIndex = (int)SceneContentControlIcons.Default;
                soundNode.SelectedImageIndex = (int)SceneContentControlIcons.Default;
                sceneNode.Nodes.Add(soundNode);
                InitializeSounds(soundNode);
            }

            sceneNode.Tag = Scene;

            Nodes.Add(sceneNode);
            sceneNode.Expand();
        }
				
        protected virtual TreeNode CreateTextureNode(a3dTextureDesc_Mapper text) {
            TreeNode texNode = new TreeNode(text.FileName);
            texNode.ImageIndex = (int)SceneContentControlIcons.Textures;
            texNode.SelectedImageIndex = (int)SceneContentControlIcons.Textures;
            texNode.Tag = text;
            return texNode;
        }

        protected virtual void InitializeTextures(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dTextureDesc_Mapper text in Scene.Textures) {
                node.Nodes.Add(CreateTextureNode(text));
            }
        }

        protected virtual TreeNode CreateLandscapeNode(a3dLandscapeDesc_Mapper land) {
            TreeNode landNode = new TreeNode(land.HeightMapFileName);
            landNode.ImageIndex = (int)SceneContentControlIcons.Lands;
            landNode.SelectedImageIndex = (int)SceneContentControlIcons.Lands;
            landNode.Tag = land;
            return landNode;
        }

        protected virtual void InitializeLandscapes(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dLandscapeDesc_Mapper land in Scene.Lands) {
                node.Nodes.Add(CreateLandscapeNode(land));
            }
        }

        protected virtual TreeNode CreateEffectNode(a3dEffectDesc_Mapper effect) {
            TreeNode effNode = new TreeNode(effect.FileName);
            effNode.ImageIndex = (int)SceneContentControlIcons.Effects;
            effNode.SelectedImageIndex = (int)SceneContentControlIcons.Effects;
            effNode.Tag = effect;
            return effNode;
        }

        protected virtual void InitializeEffects(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dEffectDesc_Mapper effect in Scene.Effects) {
                node.Nodes.Add(CreateEffectNode(effect));
            }
        }

        protected virtual TreeNode CreateModelNode(a3dModelDesc_Mapper model) {
            TreeNode modNode = new TreeNode(model.ToString());
            modNode.ImageIndex = (int)SceneContentControlIcons.Models;
            modNode.SelectedImageIndex = (int)SceneContentControlIcons.Models;
            modNode.Tag = model;
            return modNode;
        }

		protected virtual TreeNode CreateDetailNode(a3dDetail_Mapper detail) {
			TreeNode detNode = new TreeNode(detail.ToString());
            detNode.ImageIndex = (int)SceneContentControlIcons.Details;
            detNode.SelectedImageIndex = (int)SceneContentControlIcons.Details;
            detNode.Tag = detail;
			return detNode;
		}

		protected virtual void InitializeDetails(TreeNode node) {
            if (Scene == null) return;
            a3dObject_Mapper obj = node.Tag as a3dObject_Mapper;
			if(obj == null) return;
			foreach(a3dDetail_Mapper detail in obj.Details) {
				node.Nodes.Add(CreateDetailNode(detail));
			}
		}
		
		protected virtual TreeNode CreateObjectNode(a3dObject_Mapper obj) {
			TreeNode objNode = new TreeNode(obj.ToString());
            objNode.ImageIndex = (int)SceneContentControlIcons.Objects;
            objNode.SelectedImageIndex = (int)SceneContentControlIcons.Objects;
            objNode.Tag = obj;
			return objNode;
		}
		
		protected virtual void InitializeObjects(TreeNode modelNode) {
            if (Scene == null) return;
            TreeNode node = new TreeNode("Objects");
            node.ImageIndex = (int)SceneContentControlIcons.Objects;
            node.SelectedImageIndex = (int)SceneContentControlIcons.Objects;
            modelNode.Nodes.Add(node);
			a3dModelDesc_Mapper model = modelNode.Tag as a3dModelDesc_Mapper;
			if(model == null) return;
			foreach(a3dObject_Mapper obj in model.Objects) {
				TreeNode objNode = CreateObjectNode(obj);
				if(OptionsContent.ViewObjectDetails)
					InitializeDetails(objNode);
				node.Nodes.Add(objNode);
			}
		}

		protected virtual void InitializeMultiMaterialDetails(TreeNode matNode) {
            if (Scene == null) return;
		}

        protected virtual TreeNode CreateShaderParamNode(a3dShaderParamBaseMapper param) {
            TreeNode node = new TreeNode(param.ToString());
            node.ImageIndex = (int)SceneContentControlIcons.Default;
            node.SelectedImageIndex = (int)SceneContentControlIcons.Default;
            node.Tag = param;
            return node;
        }
        
        protected virtual void InitializeShaderMaterialDetails(TreeNode matNode) {
            if (Scene == null) return;
            a3dShaderMaterialMapper shaderMaterial = matNode.Tag as a3dShaderMaterialMapper;
            foreach(a3dShaderParamBaseMapper param in shaderMaterial.Params) {
                matNode.Nodes.Add(CreateShaderParamNode(param));       
            }
		}
		
		protected virtual void InitializeMaterialDetails(TreeNode matNode) {
            if (Scene == null) return;
			a3dShaderMaterialMapper shaderMat = matNode.Tag as a3dShaderMaterialMapper;
			if(shaderMat != null) {
				InitializeShaderMaterialDetails(matNode);
			}
		}
		
		protected virtual TreeNode CreateMaterialNode(a3dMaterialBaseMapper mat) {
			TreeNode matNode = new TreeNode(mat.ToString());
			matNode.Tag = mat;
            matNode.ImageIndex = (int)SceneContentControlIcons.Materials;
            matNode.SelectedImageIndex = (int)SceneContentControlIcons.Materials;
            return matNode;
		}
		
		protected virtual void InitializeMaterials(TreeNode modelNode) {
            if (Scene == null) return;
            TreeNode node = new TreeNode(MaterialsString);
			modelNode.Nodes.Add(node);
            node.ImageIndex = (int)SceneContentControlIcons.Materials;
            node.SelectedImageIndex = (int)SceneContentControlIcons.Materials;
            a3dModelDesc_Mapper model = modelNode.Tag as a3dModelDesc_Mapper;
			if(model == null) return;
			foreach(a3dMaterialBaseMapper mat in model.Materials) {
				TreeNode matNode = CreateMaterialNode(mat);
				if(OptionsContent.ViewMaterialDetails)
					InitializeMaterialDetails(matNode);
				node.Nodes.Add(matNode);
			}
		}

		protected virtual void InitializeModels(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dModelDesc_Mapper model in Scene.Models) {
				TreeNode modelNode = CreateModelNode(model);
                modelNode.ImageIndex = (int)SceneContentControlIcons.Models;
                modelNode.SelectedImageIndex = (int)SceneContentControlIcons.Models;
                if(OptionsContent.ViewModelObjects)
					InitializeObjects(modelNode);
				if(OptionsContent.ViewMaterials)
					InitializeMaterials(modelNode);
				node.Nodes.Add(modelNode);
            }
        }

        protected virtual TreeNode CreateParticleNode(a3dParticleSystemDesc_Mapper part) {
            TreeNode partNode = new TreeNode(part.Name + " ( " + part.Particle.Type + " )");
            partNode.Tag = part;
            partNode.ImageIndex = (int)SceneContentControlIcons.Particles;
            partNode.SelectedImageIndex = (int)SceneContentControlIcons.Particles;
            return partNode;
        }

        protected virtual void InitializeParticles(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dParticleSystemDesc_Mapper part in Scene.Particles) {
                node.Nodes.Add(CreateParticleNode(part));
            }
        }

        protected virtual TreeNode CreateRailNode(a3dRailSystemDesc_Mapper rail) {
            TreeNode railNode = new TreeNode(rail.RailFileName);
            railNode.Tag = rail;
            railNode.ImageIndex = (int)SceneContentControlIcons.Rails;
            railNode.SelectedImageIndex = (int)SceneContentControlIcons.Rails;
            return railNode;
        }

        protected virtual void InitializeRails(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dRailSystemDesc_Mapper rail in Scene.Rails) {
                node.Nodes.Add(CreateRailNode(rail));
            }
        }

        protected virtual TreeNode CreateLightNode(a3dLightBase_Mapper light) {
            TreeNode lightNode = new TreeNode(light.Name);
            lightNode.Tag = light;
            if (light.Type == LightType.OmniLight) {
                lightNode.ImageIndex = (int)SceneContentControlIcons.OmniLight;
                lightNode.SelectedImageIndex = (int)SceneContentControlIcons.OmniLight;
            }
            else {
                lightNode.ImageIndex = (int)SceneContentControlIcons.DirLight;
                lightNode.SelectedImageIndex = (int)SceneContentControlIcons.DirLight;
            }
            return lightNode;
        }

        protected virtual void InitializeLights(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dLightBase_Mapper light in Scene.Lights) {
                node.Nodes.Add(CreateLightNode(light));
            }
        }

        protected virtual TreeNode CreateFontNode(a3dFontDesc_Mapper font) {
            TreeNode fontNode = new TreeNode(font.FileName);
            fontNode.ImageIndex = (int)SceneContentControlIcons.Default;
            fontNode.SelectedImageIndex = (int)SceneContentControlIcons.Default;
            fontNode.Tag = font;
            return fontNode;
        }

        protected virtual void InitializeFonts(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dFontDesc_Mapper font in Scene.Fonts) {
                node.Nodes.Add(CreateFontNode(font));
            }
        }

        protected virtual TreeNode CreateSoundNode(a3dSoundDesc_Mapper sound) {
            TreeNode soundNode = new TreeNode(sound.FileName);
            soundNode.ImageIndex = (int)SceneContentControlIcons.Default;
            soundNode.SelectedImageIndex = (int)SceneContentControlIcons.Default;
            soundNode.Tag = sound;
            return soundNode;
        }

        protected virtual void InitializeSounds(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dSoundDesc_Mapper sound in Scene.Sounds) {
                node.Nodes.Add(CreateSoundNode(sound));
            }
        }

		protected virtual TreeNode CreateDetailRefNode(a3dDetailRefMapper detailRef) {
			TreeNode node = new TreeNode(detailRef.ToString());
            node.ImageIndex = (int)SceneContentControlIcons.Details;
            node.SelectedImageIndex = (int)SceneContentControlIcons.Details;
            node.Tag = detailRef;
			return node;
		}
		
		protected virtual void InitializeDetailRefs(TreeNode objRefNode) {
            if (Scene == null) return;
            a3dObjectRef_Mapper objRef = objRefNode.Tag as a3dObjectRef_Mapper;
			if(objRef == null) return;
			foreach(a3dDetailRefMapper detail in objRef.Details) {
				TreeNode detNode = CreateDetailRefNode(detail);
				objRefNode.Nodes.Add(detNode);
			}
		}

		protected virtual TreeNode CreateObjectRefNode(a3dObjectRef_Mapper obj) {
			TreeNode node = new TreeNode(obj.ToString());
            node.ImageIndex = (int)SceneContentControlIcons.Objects;
            node.SelectedImageIndex = (int)SceneContentControlIcons.Objects;
            node.Tag = obj;
			return node;
		}
		
		protected virtual void InitializeObjectRefs(TreeNode modelRefNode) {
            if (Scene == null) return;
            a3dModelRef_Mapper modelRef = modelRefNode.Tag as a3dModelRef_Mapper;
			if(modelRef == null) return;
			foreach(a3dObjectRef_Mapper obj in modelRef.Objects) {
                TreeNode objNode = CreateObjectRefNode(obj);
                if (OptionsContent.ViewDetailRefs)
                    InitializeDetailRefs(objNode); 
                modelRefNode.Nodes.Add(objNode);
			}
		}
		
		protected virtual TreeNode CreateModelRefNode(a3dModelRef_Mapper modelRef) {
            TreeNode modelRefNode = new TreeNode(modelRef.Name);
            modelRefNode.Tag = modelRef;
            modelRefNode.ImageIndex = (int)SceneContentControlIcons.ModelRefs;
            modelRefNode.SelectedImageIndex = (int)SceneContentControlIcons.ModelRefs;
            return modelRefNode;
        }

        protected virtual void InitializeModelRefs(TreeNode node) {
            if (Scene == null) return;
            foreach (a3dModelRef_Mapper modelRef in Scene.ModelRef) {
                TreeNode modelRefNode = CreateModelRefNode(modelRef);
                if (OptionsContent.ViewModelObjects)
                    this.InitializeObjectRefs(modelRefNode);
                node.Nodes.Add(modelRefNode);
            }
        }

        protected override void OnMouseDown(MouseEventArgs e) {
            base.OnMouseDown(e);
        }

        public void RefreshSceneContent() {
            OnSceneChanged();
        }

        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SceneContentControl));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "Deault_18x18.png");
            this.imageList1.Images.SetKeyName(1, "Texture_18x18.png");
            this.imageList1.Images.SetKeyName(2, "Effects_18x18.png");
            this.imageList1.Images.SetKeyName(3, "Models_18x18.png");
            this.imageList1.Images.SetKeyName(4, "Objects_18x18.png");
            this.imageList1.Images.SetKeyName(5, "Details_18x18.png");
            this.imageList1.Images.SetKeyName(6, "Materials_18x18.png");
            this.imageList1.Images.SetKeyName(7, "Lands_18x18.png");
            this.imageList1.Images.SetKeyName(8, "Particles_18x18.png");
            this.imageList1.Images.SetKeyName(9, "Rails_18x18.png");
            this.imageList1.Images.SetKeyName(10, "LineRail_18x18.png");
            this.imageList1.Images.SetKeyName(11, "RoundRail_18x18.png");
            this.imageList1.Images.SetKeyName(12, "OmniLight_18x18.png");
            this.imageList1.Images.SetKeyName(13, "DirLight_18x18.png");
            this.imageList1.Images.SetKeyName(14, "OmniLight_18x18.png");
            this.imageList1.Images.SetKeyName(15, "Models_18x18.png");
            this.ResumeLayout(false);

        }
    }

    [TypeConverter(typeof(ExpandableObjectConverter))]
    public class SceneContentOptions {
		bool viewTextures = true;
		bool viewEffects = true;
		bool viewModels = true;
		bool viewLands = true;
		bool viewParticles = true;
		bool viewRails = true;
		bool viewLights = true;
		bool viewFonts = true;
		bool viewSounds = true;
		bool viewModelRefs = true;
		bool viewModelObjects = true;
		bool viewObjectDetails = true;
		bool viewObjectRefs = true;
		bool viewDetailRefs = true;
		bool viewMaterials = true;
		bool viewMaterialDetails = true;
		bool viewModelInModelRef = true;
		bool viewObjectInObjectRef = true;
		bool viewDetailInDetailRef = true;
		bool viewMaterialsInMultiMaterial = true;
		SceneContentControl sceneControl;

		public SceneContentOptions(SceneContentControl sceneControl) {
			this.sceneControl = sceneControl;
		}

        [Browsable(false)]
        public SceneContentControl SceneControl { get { return sceneControl; } }
        
        protected virtual void OnPropertiesChanged() {
            if (SceneControl == null) return;
            SceneControl.RefreshSceneContent();
        }

        [DefaultValue(true)]
		public virtual bool ViewTextures { 
            get { return viewTextures; } 
            set {
                if (ViewTextures == value) return;
                viewTextures = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewEffects { 
            get { return viewEffects; } 
            set {
                if (ViewEffects == value) return;
                viewEffects = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewModels { 
            get { return viewModels; } 
            set {
                if (ViewModels == value) return;
                viewModels = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewLands { 
            get { return viewLands; } 
            set {
                if (ViewLands == value) return;
                viewLands = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewParticles { 
            get { return viewParticles; } 
            set {
                if (ViewParticles == value) return;
                viewParticles = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewRails { 
            get { return viewRails; } 
            set {
                if (ViewRails == value) return;
                viewRails = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewLights { 
            get { return viewLights; } 
            set {
                if (ViewLights == value) return;
                viewLights = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewFonts { 
            get { return viewFonts; } 
            set {
                if (ViewFonts == value) return;
                viewFonts = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewSounds { 
            get { return viewSounds; } 
            set {
                if (ViewSounds == value) return;
                viewSounds = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewModelRefs { 
            get { return viewModelRefs; } 
            set {
                if (ViewModelRefs == value) return;
                viewModelRefs = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewModelObjects { 
            get { return viewModelObjects; } 
            set {
                if (ViewModelObjects == value) return;
                viewModelObjects = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewObjectDetails { 
            get { return viewObjectDetails; } 
            set {
                if (ViewObjectDetails == value) return;
                viewObjectDetails = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewObjectRefs { 
            get { return viewObjectRefs; } 
            set {
                if (ViewObjectRefs == value) return;
                viewObjectRefs = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewDetailRefs { 
            get { return viewDetailRefs; } 
            set {
                if (ViewDetailRefs == value) return;
                viewDetailRefs = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewMaterials { 
            get { return viewMaterials; } 
            set {
                if (ViewMaterials == value) return;
                viewMaterials = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewMaterialDetails { 
            get { return viewMaterialDetails; } 
            set {
                if (ViewMaterialDetails == value) return; 
                viewMaterialDetails = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewModelInModelRef { 
            get { return viewModelInModelRef; } 
            set {
                if (ViewModelInModelRef) return;
                viewModelInModelRef = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewObjectInObjectRef { 
            get { return viewObjectInObjectRef; } 
            set {
                if (ViewObjectInObjectRef == value) return; 
                viewObjectInObjectRef = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewDetailInDetailRef { 
            get { return viewDetailInDetailRef; } 
            set {
                if (ViewDetailInDetailRef == value) return;
                viewDetailInDetailRef = value;
                OnPropertiesChanged();
            } 
        }
		[DefaultValue(true)]
		public virtual bool ViewMaterialsInMultiMaterial { 
            get { return viewMaterialsInMultiMaterial; } 
            set {
                if (ViewMaterialsInMultiMaterial == value) return;
                viewMaterialsInMultiMaterial = value;
                OnPropertiesChanged();
            } 
        }
	}

    public enum SceneContentControlIcons { 
        Default = 0,
        Textures = 1,
        Effects = 2,
        Models = 3,
        Objects = 4,
        Details = 5, 
        Materials = 6,
        Lands = 7,
        Particles = 8,
        Rails = 9,
        LineRail = 10,
        RoundRail = 11,
        OmniLight = 12,
        DirLight = 13,
        Lights = 14,
        ModelRefs = 15
    }
}