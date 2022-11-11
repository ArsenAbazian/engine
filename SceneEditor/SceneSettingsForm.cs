using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor {
    public partial class SceneSettingsForm : FormWithSceneUpdate {
		public SceneSettingsForm() {
			InitializeComponent();
		}

		public SceneSettingsForm(MainForm form) : this() {
            MainForm = form;
		}

        public string SceneDataDirectory { get { return sceneSettings1.SceneDataDirectory; } set { sceneSettings1.SceneDataDirectory = value; } }
        public string TextureDirectory { get { return sceneSettings1.TextureDirectory; } set { sceneSettings1.TextureDirectory = value; } }
        public string EffectDirectory { get { return sceneSettings1.EffectDirectory; } set { sceneSettings1.EffectDirectory = value; } }
        public string ModelDirectory { get { return sceneSettings1.ModelDirectory; } set { sceneSettings1.ModelDirectory = value; } }
        public string LandDirectory { get { return sceneSettings1.LandDirectory; } set { sceneSettings1.LandDirectory = value; } }
        public string ParticleDirectory { get { return sceneSettings1.ParticleDirectory; } set { sceneSettings1.ParticleDirectory = value; } }
        public string RailDirectory { get { return sceneSettings1.RailDirectory; } set { sceneSettings1.RailDirectory = value; } }
        public string FontDirectory { get { return sceneSettings1.FontDirectory; } set { sceneSettings1.FontDirectory = value; } }
        public string SoundDirectory { get { return sceneSettings1.SoundDirectory; } set { sceneSettings1.SoundDirectory = value; } }
	}
}