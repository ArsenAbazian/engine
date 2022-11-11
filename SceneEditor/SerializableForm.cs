using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using sceneEditor.Configuration;
using System.Drawing;

namespace sceneEditor {
    public class SerializableForm : Form {

        protected override void OnShown(EventArgs e) {
            base.OnShown(e);
            ApplySettings();
        }
        protected virtual void ApplySettings() {
            if (DesignMode) return;
            Size = (Size)Configurator.Default.Options[Name].GetValue("Size", Size);
            Location = (Point)Configurator.Default.Options[Name].GetValue("Location", Location);
        }
        protected virtual void SaveSettings() {
            if (DesignMode) return;
            Configurator.Default.SetValue(Name, "Size", Size);
            Configurator.Default.SetAndSave(Name, "Location", Location);
        }
        protected override void OnVisibleChanged(EventArgs e) {
            base.OnVisibleChanged(e);
            if (!Visible) {
                SaveSettings();
            }
        }
        protected override void OnClosed(EventArgs e) {
            base.OnClosed(e);
            SaveSettings();
        }
    }
}
