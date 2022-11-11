using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;
using System.Collections;

namespace sceneEditor {
    public partial class AddModelRefControl : ControlWithSceneUpdate {
        public AddModelRefControl() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            OnSceneChanged();
        }

        protected virtual void OnSceneChanged() {
            this.modelsList.Items.Clear();
            if (Scene == null)
                return;
            IList mList = Scene.Models;
            if (mList == null) return;
        
            for( int i = 0; i < Scene.Models.Count; i++ ) {
                this.modelsList.Items.Add(new ModelItem( mList[i] as a3dModelDesc_Mapper ));
            }
        }

        public event EventHandler OkClick {
            add { okButton.Click += value; }
            remove { okButton.Click -= value; }
        }

        protected virtual void AddModelRef() {
            if (modelsList.SelectedItem == null) return;
            using (EnterNameForm form = new EnterNameForm()) {
                if (form.ShowDialog() == DialogResult.OK) {
                    ModelItem item = modelsList.SelectedItem as ModelItem;
                    if (item == null) return;
                    Scene.AddModelRef(item.ModelDesc);
                    Scene.ModelRef[Scene.ModelRef.Count - 1].Name = form.Control.Value;
                    MainForm.SolutionControl.SceneView.RefreshSceneContent();
                }
            }
        }
        
        private void addButton_Click(object sender, EventArgs e) {
            AddModelRef();
        }

        private void modelsList_DoubleClick(object sender, EventArgs e) {
            AddModelRef();
        }
    }

    public class ModelItem {
        a3dModelDesc_Mapper modelDesc;
    
        public ModelItem( a3dModelDesc_Mapper modelDesc ) { 
            this.modelDesc = modelDesc;
        }

        public a3dModelDesc_Mapper ModelDesc { get { return modelDesc; } }

        public override string ToString() {
            if (ModelDesc == null) return "empty";
            if (ModelDesc.Name != null && ModelDesc.Name != string.Empty)
                return ModelDesc.Name;
            return modelDesc.FileName;
        }
    } 
}
