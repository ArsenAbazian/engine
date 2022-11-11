using System;
using System.Collections.Generic;
using System.Text;
using classMapper;
using System.Windows.Forms;

namespace sceneEditor.Dependency {
    
    class DependencyTree : TreeView {
        IDependencyTreeNode rootNode;
        
        public DependencyTree() : base() { }

        public IDependencyTreeNode RootNode {
            get { return rootNode; }
            set {
                if (RootNode == value) return;
                rootNode = value;
                OnRootNodeChanged();
            }
        }

        protected virtual void OnRootNodeChanged() {
            Nodes.Clear();
            Nodes.Add(CreateNode(RootNode));
            CreateNodesFor(RootNode, Nodes[0]);
        }

        protected virtual TreeNode CreateNode(IDependencyTreeNode node) { 
            string nodeName = node.DependencyOwner == null? node.NodeName: node.DependencyOwner.DependencyOwnerName;
            TreeNode tnode = new TreeNode(nodeName);
            tnode.Tag = node;
            return tnode;
        }

        protected virtual void CreateNodesFor(IDependencyTreeNode node, TreeNode tnode) {
            if (node.Children == null) return;
            foreach (IDependencyTreeNode child in node.Children) {
                TreeNode childNode = CreateNode(child);
                CreateNodesFor(child, childNode);
                tnode.Nodes.Add(childNode);
            }
        }
        protected override void OnBeforeExpand(TreeViewCancelEventArgs e) {
            base.OnBeforeExpand(e);
            foreach (TreeNode node in e.Node.Nodes) {
                if (node.Nodes.Count == 0) 
                    CreateNodesFor(node.Tag as IDependencyTreeNode, node);
            }
        }

        protected override void OnMouseMove(MouseEventArgs e) {
            base.OnMouseMove(e);
            TreeNode node = GetNodeAt(e.Location);
            if(node != null && Control.ModifierKeys == Keys.Control)
                node.Expand();
        }

        private void InitializeComponent() {
            this.SuspendLayout();
            // 
            // DependencyTree
            // 
            this.ResumeLayout(false);

        }
    }
}
