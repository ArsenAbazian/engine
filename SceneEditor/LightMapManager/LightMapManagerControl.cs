using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.LightMapManager {
    public partial class LightMapManagerControl : ControlWithSceneUpdate {
        public LightMapManagerControl() {
            InitializeComponent();
        }

        private void checkAllButton_Click(object sender, EventArgs e) {
            RecursiveSetCheck(sceneContentControl1.Nodes, true);              
        }

        private void clearAllButton_Click(object sender, EventArgs e) {
            RecursiveSetCheck(sceneContentControl1.Nodes, false);
        }

        protected virtual void RecursiveSetCheck(TreeNodeCollection nodes, bool check) {
            foreach (TreeNode node in nodes) {
                if (sceneContentControl1.CanSetLightMapCheck(node))
                    node.Checked = check;
                RecursiveSetCheck(node.Nodes, check);
            }
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            sceneContentControl1.Scene = MainForm.Scene;
        }

        private void sceneContentControl1_BeforeCheck(object sender, TreeViewCancelEventArgs e) {
            if (!sceneContentControl1.CanSetLightMapCheck(e.Node)) e.Cancel = true;
        }

        private void sceneContentControl1_AfterSelect(object sender, TreeViewEventArgs e) {
            if (sceneContentControl1.SelectedNode == null) return;
            ILightMapInfo_MapperProvider provider = sceneContentControl1.SelectedNode.Tag as ILightMapInfo_MapperProvider;
            if (provider == null)
                lightMapInfoControl1.LightMapInfo = null;
            else
                lightMapInfoControl1.LightMapInfo = provider.LightMapInfo;
        }
    }
}
