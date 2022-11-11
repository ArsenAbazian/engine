using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using classMapper;

namespace sceneEditor.Dependency {
    public partial class DependencyValueControl : UserControl {
        private static readonly object valueChanged = new object();
        
        public DependencyValueControl() {
            InitializeComponent();
        }

        a3dDependencyPropertyMapper property;
        public a3dDependencyPropertyMapper Property {
            get { return property; }
            set {
                if (Property == value) return;
                property = value;
                OnPropertyChanged();
            }
        }

        protected virtual void OnPropertyChanged() {
            this.comboBox1.Visible = false;
            this.valueTextBox1.Visible = false;

            if(Property == null)
                return;

            if(Property.PropertyType == a3dDependencyPropertyTypeMapper.Bool) {
                this.comboBox1.SelectedIndex = Property.BoolValue ? 0 : 1;
                this.comboBox1.Visible = true;
            }
            else if(Property.PropertyType == a3dDependencyPropertyTypeMapper.Int) {
                this.valueTextBox1.Visible = true;
                this.valueTextBox1.Text = Property.IntValue.ToString();
            }
            else if(Property.PropertyType == a3dDependencyPropertyTypeMapper.Float) {
                this.valueTextBox1.Visible = true;
                this.valueTextBox1.Text = Property.FloatValue.ToString();
            }
        }

        private void valueTextBox1_ValueChanged(object sender, EventArgs e) {
            if (Property.PropertyType == a3dDependencyPropertyTypeMapper.Int)
                Property.IntValue = int.Parse(valueTextBox1.Text);
            else if (Property.PropertyType == a3dDependencyPropertyTypeMapper.Float)
                Property.FloatValue = float.Parse(valueTextBox1.Text);
            RaiseValueChanged();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e) {
            Property.BoolValue = comboBox1.SelectedIndex == 0 ? true : false;
            RaiseValueChanged();
        }

        protected virtual void RaiseValueChanged() {
            EventHandler handler = Events[valueChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }

        public event EventHandler ValueChanged {
            add { Events.AddHandler(valueChanged, value); }
            remove { Events.RemoveHandler(valueChanged, value); }
        }
    }
}
