using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class ModelRefCollectionViewControl : ControlWithSceneUpdate {
        public ModelRefCollectionViewControl() {
            InitializeComponent();
        }

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            OnSceneChanged();
        }

        protected virtual void OnSceneChanged() {

            this.modelRefList.Items.Clear();
            if( Scene == null ) return;
            foreach( a3dModelRef_Mapper modelRef in Scene.ModelRef ) {
                this.modelRefList.Items.Add( new ModelRefInfo( modelRef ) );
            }

            return;
        }

        private void removeButton_Click(object sender, EventArgs e) {
            foreach( ModelRefInfo info in this.modelRefList.CheckedItems) {
                Scene.RemoveModelRef(info.ModelRef);
            }
            while(this.modelRefList.CheckedItems.Count > 0) { 
                this.modelRefList.Items.Remove(this.modelRefList.CheckedItems[0]);
            }
        }

        public event EventHandler OkClick {
            add { OK.Click += value; }
            remove { OK.Click += value; }
        }
    }

    public class ModelRefInfo {
        a3dModelRef_Mapper modelRef;
        
        public ModelRefInfo( a3dModelRef_Mapper modelRef ) {
            this.modelRef = modelRef;
        }

        public a3dModelRef_Mapper ModelRef { get { return modelRef; } }
        public override string ToString() {
            if(ModelRef == null) return base.ToString();
            string s = ModelRef.Name;
            if( ModelRef.ModelDesc == null ) return s;
            return s + "( " + ModelRef.ModelDesc.FileName + " )";
        }
    }
}
