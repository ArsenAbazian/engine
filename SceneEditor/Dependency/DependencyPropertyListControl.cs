using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;
using sceneEditor.InheritedClassMapper;

namespace sceneEditor.Dependency {
    public partial class DependencyPropertyListControl : ControlWithSceneUpdate {
        public DependencyPropertyListControl() {
            InitializeComponent();
        }

        IDependencyPropertyOwner propertyOwner;
        public IDependencyPropertyOwner PropertyOwner {
            get { return propertyOwner; }
            set { 
                if(PropertyOwner == value) return;
                propertyOwner = value;
                OnPropertyOwnerChanged();
            }
        }

        protected virtual void OnPropertyOwnerChanged() {
            this.dependencyPropertyList.Items.Clear();
            if(PropertyOwner == null) return;
            foreach (a3dDependencyPropertyMapper prop in PropertyOwner.DependencyProperties) {
                this.dependencyPropertyList.Items.Add(new DependencyPropertyInfo(prop));
            }
        }

        public DependencyPropertyInfo SelectedPropertyInfo { get { return dependencyPropertyList.SelectedItem as DependencyPropertyInfo; } }

        DependencyPropertyControl depPropControl;
        public DependencyPropertyControl  PropertyControl {
            get { return depPropControl; }
            set { depPropControl = value; }
        }

        private void dependencyPropertyList_SelectedIndexChanged(object sender, EventArgs e) {
            dependencyValueControl1.Property = SelectedPropertyInfo == null? null: SelectedPropertyInfo.Property;
        }

        private void dependencyValueControl1_ValueChanged(object sender, EventArgs e) {
            dependencyPropertyList.Items[dependencyPropertyList.SelectedIndex] = new DependencyPropertyInfo(SelectedPropertyInfo.Property);
        }

        private void showDependencyGraphToolStripMenuItem_Click(object sender, EventArgs e) {
            Hide();
            try {
                if(SelectedPropertyInfo == null) return;
                using(DependencyGraphViewerForm form = new DependencyGraphViewerForm()) {
                    form.MainForm = MainForm;
                    form.RootNode = MainForm.Scene;
                    form.Property = SelectedPropertyInfo.Property;
                    form.ShowDialog();
                }
            }
            finally {
                Show();
            }
        }
    }

    public class DependencyPropertyInfo {
        a3dDependencyPropertyMapper property;
        public DependencyPropertyInfo( a3dDependencyPropertyMapper property ) {
            this.property = property;
        }

        public a3dDependencyPropertyMapper Property { get { return property; } }

        public override string ToString() {
            string res = string.Empty;
            if (Property == null)
                return base.ToString();
            //if (Property.Graph != null)
            //    res += "* ";
            if (Property.PropertyType == a3dDependencyPropertyTypeMapper.Bool)
                res = Property.PropertyName + " = " + Property.BoolValue.ToString();
            else if (Property.PropertyType == a3dDependencyPropertyTypeMapper.Int)
                res = Property.PropertyName + " = " + Property.IntValue.ToString();
            else if (Property.PropertyType == a3dDependencyPropertyTypeMapper.Float)
                res = Property.PropertyName + " = " + Property.FloatValue.ToString();
            else
                res = Property.PropertyName + " = none";
            return res;
        }
    }
}
