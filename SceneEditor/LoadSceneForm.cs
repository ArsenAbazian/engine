using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor
{
    public partial class LoadSceneForm : FormWithSceneUpdate
    {
        public LoadSceneForm()
        {
            InitializeComponent();
        }

        public string SceneFileName { get { return loadSceneControl1.SceneFileName; } set { loadSceneControl1.SceneFileName = value; } }
        public string DataDirectoryName { get { return loadSceneControl1.DataDirectoryName; } set { loadSceneControl1.DataDirectoryName = value; } }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            loadSceneControl1.MainForm = MainForm;
        }
    }
}