using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor
{
    public partial class LoadSceneControl : ControlWithSceneUpdate
    {
        public LoadSceneControl()
        {
            InitializeComponent();
        }

        public string SceneFileName { get { return this.sceneFileText.Text; } set { this.sceneFileText.Text = value; } }
        public string DataDirectoryName { get { return this.dataDirText.Text; } set { this.dataDirText.Text = value; } }

        private void sceneFileButton_Click(object sender, EventArgs e)
        {
            openSceneDialog.ShowDialog();
        }

        private void dataDirButton_Click(object sender, EventArgs e)
        {
            if(dataDirDialog.ShowDialog() == DialogResult.OK ) {
                this.dataDirText.Text = dataDirDialog.SelectedPath;
            }
        }

        private void openSceneDialog_FileOk(object sender, CancelEventArgs e)
        {
            this.sceneFileText.Text = openSceneDialog.FileName;
        }
    }
}
