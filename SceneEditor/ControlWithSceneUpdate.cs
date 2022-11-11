using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor {
    public partial class ControlWithSceneUpdate : UserControl {
        public ControlWithSceneUpdate() {
            InitializeComponent();
        }

        MainForm mainForm;
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public MainForm MainForm {
            get {
                //if (mainForm == null) {
                //    MainForm frm = RetreiveMainForm();
                //    OnMainFormChanging();
                //    mainForm = mainForm;
                //    OnMainFormChanged();
                //}
                return mainForm; 
            }
            set {
                OnMainFormChanging();
                mainForm = value;
                OnMainFormChanged();
            }
        }

        MainForm RetreiveMainForm() {
            Control ctrl = Parent;
            while (ctrl != null) {
                ControlWithSceneUpdate cwsu = ctrl as ControlWithSceneUpdate;
                FormWithSceneUpdate frm = ctrl as FormWithSceneUpdate;
                if (cwsu != null) return cwsu.MainForm;
                else if (frm != null) return frm.MainForm;
                ctrl = ctrl.Parent;
            }
            return null;
        }

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public a3dSceneMapper Scene {
            get {
                if (MainForm == null) return null;
                return MainForm.Scene;
            }
        }

        protected virtual void OnMainFormChanging() {
        }

        protected virtual void OnMainFormChanged() { 
        }

        protected override void OnLoad(EventArgs e) {
            base.OnLoad(e);
            /*
            if (MainForm == null && !DesignMode ) {
                throw new Exception(this.GetType().ToString() + ": MainForm property == null");
            }*/
        }

        public virtual void UpdateMainForm() {
            MainForm.UpdateForm();
        }
    }
}
