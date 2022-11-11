using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;
using sceneEditor.ShaderParams;

namespace sceneEditor {
    public partial class SolutionControl : ControlWithSceneUpdate {
        public SolutionControl() {
            InitializeComponent();
            SceneView.Nodes[0].Expand();
        }

        public SceneContentControl SceneView { get { return sceneView; } }

        private void sceneView_BeforeCollapse(object sender, TreeViewCancelEventArgs e) {
            if (e.Node.Text == SceneView.SceneString) e.Cancel = true;
        }

        public event TreeViewEventHandler AfterSelectItem {
            add { SceneView.AfterSelect += value; }
            remove { SceneView.AfterSelect -= value; }
        }

        protected override void OnMainFormChanging() {
            base.OnMainFormChanging();
            if (MainForm != null) MainForm.Scene.SceneChanged -= new EventHandler(OnSceneChanged);
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            if (MainForm != null) {
                MainForm.Scene.SceneChanged += new EventHandler(OnSceneChanged);
                SceneView.Scene = MainForm.Scene;
                OnSceneChanged(MainForm.Scene, EventArgs.Empty);
            }
        }

        protected virtual void OnSceneChanged(object sender, EventArgs e) {
            SceneView.RefreshSceneContent();
        }

        private void sceneView_MouseDown(object sender, MouseEventArgs e) {
            if (e.Button != MouseButtons.Right) return;
            SceneView.SelectedNode = SceneView.GetNodeAt(e.Location);
            UpdateMenu();
            menu.Show(SceneView.PointToScreen(e.Location));
        }

        protected virtual void UpdateMenu() {
            foreach (ToolStripMenuItem item in menu.Items) {
                item.Visible = true;
                item.Enabled = true;
            }
            if (SceneView.SelectedMaterial == null) {
                changeToolStripMenuItem.Visible = false;
            }
            if (!MainForm.IsSceneValid) {
                addMenuItem.Enabled = false;
                propMenuItem.Enabled = false;
                changeToolStripMenuItem.Enabled = false;
                removeMenuItem.Enabled = false;
            }
            dependencyMenuItem.Enabled = GetDependencyPropertyOwner() != null;
        }

        private void updateViewMenuItem_Click(object sender, EventArgs e) {
            OnSceneChanged(this, EventArgs.Empty);
        }

        private void propMenuItem_Click(object sender, EventArgs e) {
            if (SceneView.SelectedNode == null) return;

            a3dLandscapeDesc_Mapper land = SceneView.SelectedNode.Tag as a3dLandscapeDesc_Mapper;
            if (land != null) {
                MainForm.LandPropsForm.Landscape = land;
                MainForm.LandPropsForm.Show();
                return;
            }

            if (SceneView.IsDetailRefNode) {
                ShaderParamsForm form = new ShaderParamsForm();
                form.MainForm = MainForm;
                form.ParamsControl.ParamCollection = SceneView.SelectedDetailRef.Material.Params;
                form.Show();
                return;
            }

            if (SceneView.IsMaterialNode) {
                ShaderParamsForm form = new ShaderParamsForm();
                form.MainForm = MainForm;
                form.ParamsControl.ParamCollection = SceneView.SelectedMaterial.Params;
                form.Show();
                form.ModelDesc = SceneView.SelectedNode.Parent.Parent.Tag as a3dModelDesc_Mapper;
                return;
            }

            propGrid.SelectedObject = SceneView.SelectedNode.Tag;
            tabControl.SelectedTab = tabControl.TabPages[1];
        }



        private void addMenuItem_Click(object sender, EventArgs e) {
            if (SceneView.SelectedNode == null) return;

            if (SceneView.IsTexturesNode) {
                MainForm.AddTexture();
            }

            if (SceneView.IsLandscapesNode) {
                MainForm.AddLandForm.ShowDialog();
                return;
            }

            if (SceneView.IsModelRefsNode) {
                MainForm.AddModelRefForm.ShowDialog();
                return;
            }

            if (SceneView.IsModelsNode) {
                MainForm.AddModel();
            }

            if (SceneView.IsEffectsNode) {
                MainForm.AddEffect();
            }

            if (SceneView.IsLightsNode) {
                MainForm.AddLight();
            }

            if (SceneView.IsParticlesNode) {
                MainForm.AddParticleSystem();
            }
        }

        private void sceneView_AfterSelect(object sender, TreeViewEventArgs e) {
            MainForm.UpdateLightMapGrid();
            MainForm.UpdateBasisRender();
        }

        private void changeToolStripMenuItem_Click(object sender, EventArgs e) {
            if (SceneView.SelectedMaterial != null) {
                MainForm.EffectListForm.Show();
            }
        }

        protected virtual void RecursiveCollapseNode(TreeNode node) {
            if (!node.IsExpanded) return;
            foreach (TreeNode n in node.Nodes) {
                RecursiveCollapseNode(n);
            }
            node.Collapse();
        }

        protected virtual void RecursiveExpandNode(TreeNode node) {
            node.Expand();
            foreach (TreeNode n in node.Nodes) {
                RecursiveExpandNode(n);
            }
        }

        protected virtual void RecursiveCollapse() {
            RecursiveCollapseNode(SceneView.SelectedNode);
        }

        protected virtual void RecursiveExpand() {
            RecursiveExpandNode(SceneView.SelectedNode);
        }

        private void fullExpandCollapseMenuItem_Click(object sender, EventArgs e) {
            TreeNode node = SceneView.SelectedNode;
            if (node == null) return;
            if (node.IsExpanded) RecursiveCollapse();
            else RecursiveExpand();
        }

        protected IDependencyPropertyOwner GetDependencyPropertyOwner() { 
            if (SceneView.SelectedParticle != null) return SceneView.SelectedParticle.Particle as IDependencyPropertyOwner;
            return null;
        }

        private void dependencyMenuItem_Click(object sender, EventArgs e) {
            MainForm.ShowDependencyPropertyForm(GetDependencyPropertyOwner());
        }
    }
}
