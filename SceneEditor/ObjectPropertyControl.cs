using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class ObjectPropertyControl : ControlWithSceneUpdate {
		public ObjectPropertyControl() {
			InitializeComponent();
		}

		public PropertyGrid ObjectProperty { get { return objectProperty; } }
	}
}
