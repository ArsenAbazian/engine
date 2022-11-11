using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class FormWithSceneUpdate : SerializableForm {
        public FormWithSceneUpdate() {
            InitializeComponent();
        }

        MainForm mainForm;
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]                    
        public MainForm MainForm {
            get { return mainForm; }
            set {
                mainForm = value;
                OnMainFormChanged();
            }
        }

        protected virtual void OnMainFormChanged() { 
        }

        public a3dSceneMapper Scene {
            get {
                if (MainForm == null) return null;
                return MainForm.Scene;
            }
        }

        protected override void OnLoad(EventArgs e) {
            base.OnLoad(e);
            if (MainForm == null && !DesignMode) {
                throw new Exception(this.GetType().ToString() + ": MainForm property == null");
            }
        }

        protected virtual void UpdateMainForm() {
            MainForm.UpdateScene();
            MainForm.ViewControl.Invalidate();
        }
    }
}