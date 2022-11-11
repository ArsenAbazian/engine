using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class SceneHierarchyControl : ControlWithSceneUpdate {
		public SceneHierarchyControl() {
			InitializeComponent();
			SceneView.Nodes[0].Expand();
		}

		public TreeView SceneView { get { return sceneView;  } }

		protected virtual Rectangle GetNodeTextRect(DrawTreeNodeEventArgs e) {
			return new Rectangle(e.Node.Bounds.X, e.Node.Bounds.Y, e.Node.Bounds.Width + 2, e.Node.Bounds.Height);
		}
		protected virtual Rectangle GetNodeBackgroundRect(DrawTreeNodeEventArgs e) {
			return new Rectangle(e.Bounds.X, e.Node.Bounds.Y, e.Bounds.Right - e.Bounds.X, e.Node.Bounds.Height);
		}
		protected virtual Rectangle GetVerticalLineRect(DrawTreeNodeEventArgs e) { 
			Rectangle rect = Rectangle.Empty;
			rect.X = e.Node.Bounds.X - 5;
			rect.Y = e.Node.Bounds.Y;
			rect.Width = 0;
			if(e.Node.NextNode == null) rect.Height = e.Node.Bounds.Height / 2;
			else rect.Height = e.Node.Bounds.Height;
			return rect;
		}
		private void sceneView_DrawNode(object sender, DrawTreeNodeEventArgs e) {
			if(e.Node.IsSelected) {
				e.Graphics.FillRectangle(Brushes.Red, GetNodeBackgroundRect(e));
			}
			e.Graphics.DrawString(e.Node.Text, SceneView.Font, new SolidBrush(SceneView.ForeColor), GetNodeTextRect(e));
			if((SceneView.ShowRootLines && e.Node.Parent == null) || e.Node.Parent != null) {
				Rectangle rect = GetVerticalLineRect(e);
				e.Graphics.DrawLine(new Pen(SceneView.LineColor), rect.Location, new Point(rect.Right, rect.Bottom));
			}
		}

		private void sceneView_BeforeCollapse(object sender, TreeViewCancelEventArgs e) {
			if(e.Node.Text == "Scene") e.Cancel = true;
		}

		public event TreeViewEventHandler AfterSelectItem {
			add { SceneView.AfterSelect += value; }
			remove { SceneView.AfterSelect -= value; }
		}

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            OnSceneChanged();
        }
		protected virtual void OnSceneChanged() {
			InitializeTreeView();
		}

		protected virtual void ClearTreeView() {
			SceneView.Nodes.Clear();
		}
		
		protected virtual void InitializeTreeView() {
			ClearTreeView();
			
			TreeNode texNode = new TreeNode("Textures");
			TreeNode effNode = new TreeNode("Effects");
			TreeNode modNode = new TreeNode("Models");
			TreeNode landNode = new TreeNode("Landscapes");
			TreeNode partNode = new TreeNode("Particle Systems");
			TreeNode railNode = new TreeNode("Railway Systems");
			TreeNode lightNode = new TreeNode("Lights");
			TreeNode fontNode = new TreeNode("Fonts");
			TreeNode soundNode = new TreeNode("Sounds");
            TreeNode refNode = new TreeNode("ModelRef");
			TreeNode sceneNode = new TreeNode("Scene");
			
            sceneNode.Nodes.Add(texNode);
            InitializeTextures(texNode);

            sceneNode.Nodes.Add(effNode);
            InitializeEffects(effNode);

            sceneNode.Nodes.Add(modNode);
            InitializeModels(modNode);
			
            sceneNode.Nodes.Add(landNode);
            InitializeLandscapes(landNode);    

            sceneNode.Nodes.Add(partNode);
            InitializeParticles(partNode);

            sceneNode.Nodes.Add(railNode);
            InitializeRails(railNode);
			
            sceneNode.Nodes.Add(lightNode);
            InitializeLights(lightNode);

            sceneNode.Nodes.Add(refNode);
            InitializeModelRefs(refNode);
            
            sceneNode.Nodes.Add(fontNode);
            InitializeFonts(fontNode);
			
            sceneNode.Nodes.Add(soundNode);
            InitializeSounds(soundNode);

			sceneNode.Tag = Scene;

			SceneView.Nodes.Add(sceneNode);
			sceneNode.Expand();
		}

        protected virtual void InitializeTextures(TreeNode node) { 
            foreach(a3dTextureDesc_Mapper text in Scene.Textures) {
                TreeNode texNode = new TreeNode(text.FileName);
                texNode.Tag = text;
                node.Nodes.Add(texNode);
            }
        }
        
        protected virtual void InitializeLandscapes(TreeNode node) { 
            foreach(a3dLandscapeDesc_Mapper land in Scene.Lands) {
                TreeNode landNode = new TreeNode(land.HeightMapFileName);
                landNode.Tag = land;
                node.Nodes.Add(landNode);
            }
        }

        protected virtual void InitializeEffects(TreeNode node) { 
            foreach(a3dEffectDesc_Mapper effect in Scene.Effects) {
                TreeNode effNode = new TreeNode(effect.FileName);
                effNode.Tag = effect;
                node.Nodes.Add(effNode);
            }
        }

        protected virtual void InitializeModels(TreeNode node) {
            foreach (a3dModelDesc_Mapper model in Scene.Models) {
                TreeNode modNode = new TreeNode(model.FileName);
                modNode.Tag = model;
                node.Nodes.Add(modNode);
            }
        }

        protected virtual void InitializeParticles(TreeNode node) { 
            foreach (a3dParticleSystemDesc_Mapper part in Scene.Particles) {
                TreeNode partNode = new TreeNode(part.Name + " ( " + part.EffectFileName + " )");
                partNode.Tag = part;
                node.Nodes.Add(partNode);
            }
        }

        protected virtual void InitializeRails(TreeNode node) {
            foreach (a3dRailSystemDesc_Mapper rail in Scene.Rails) {
                TreeNode railNode = new TreeNode(rail.RailFileName);
                railNode.Tag = rail;
                node.Nodes.Add(railNode);
            }
        }

        protected virtual void InitializeLights(TreeNode node) {
            foreach (a3dLightBase_Mapper light in Scene.Lights) {
                TreeNode lightNode = new TreeNode(light.Name);
                lightNode.Tag = light;
                node.Nodes.Add(lightNode);
            }
        }

        protected virtual void InitializeFonts(TreeNode node) { 
            foreach ( a3dFontDesc_Mapper font in Scene.Fonts ) {
                TreeNode fontNode = new TreeNode(font.FileName);
                fontNode.Tag = font;
                node.Nodes.Add(fontNode);
            }
        }

        protected virtual void InitializeSounds(TreeNode node) {
            foreach (a3dSoundDesc_Mapper sound in Scene.Sounds) {
                TreeNode soundNode = new TreeNode(sound.FileName);
                soundNode.Tag = sound;
                node.Nodes.Add(soundNode);
            }
        }

        protected virtual void InitializeModelRefs(TreeNode node) {
            foreach (a3dModelRef_Mapper modelRef in Scene.ModelRef) {
                TreeNode modelRefNode = new TreeNode(modelRef.Name);
                modelRefNode.Tag = modelRef;
                node.Nodes.Add(modelRefNode);
            }
        }
	}
}
