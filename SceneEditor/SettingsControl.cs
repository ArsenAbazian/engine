using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using sceneEditor.Configuration;

namespace sceneEditor {
    public partial class SettingsControl : ControlWithSceneUpdate {
		public SettingsControl() {
			InitializeComponent();
		}

		public SettingsControl(MainForm mainForm) : this() {
            MainForm = MainForm;

			if(MainForm != null)
				InitializeObjectList();	
		}

        protected override void OnMainFormChanged() {
            base.OnMainFormChanged();
            InitializeObjectList();
        }

		private void objectList_SelectedIndexChanged(object sender, EventArgs e) {
			this.objectProp.SelectedObject = (this.objectList.SelectedItem as ObjectListItem).ContextData;
            UpdateMainForm();
        }
        
        protected override void OnVisibleChanged(EventArgs e) {
            base.OnVisibleChanged(e);
        }

		public ComboBox ObjectList { get { return objectList; } }
		public PropertyGrid ObjectProp { get { return objectProp; } }

		protected virtual void InitializeObjectList() {
			ObjectList.Items.Clear();
			
			ObjectList.Items.Add(new ObjectListItem( "Kernel", MainForm.Kernel ) );
			ObjectList.Items.Add(new ObjectListItem( "Grid", MainForm.Grid ) );
			ObjectList.Items.Add(new ObjectListItem( "Default Camera", MainForm.Camera ) );
            ObjectList.Items.Add(new ObjectListItem( "LightMapGrid", MainForm.LightMapGrid));

			ObjectList.SelectedIndex = 0;
		}

        private void objectProp_PropertyValueChanged(object s, PropertyValueChangedEventArgs e) {
            UpdateMainForm();
        }
	}

	public class ObjectListItem {
		string itemName;
		object contextData;

		public ObjectListItem(string itemName, object contextData) {
			this.itemName = itemName;
			this.contextData = contextData;
		}

		public string ItemName { get { return itemName; } set { itemName = value; } }
		public object ContextData { get { return contextData; } set { contextData = value; } }

		public override string ToString() {
			return ItemName;
		}
	}
}
