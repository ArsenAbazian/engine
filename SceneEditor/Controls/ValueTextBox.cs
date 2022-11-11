using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace sceneEditor.Controls {
    public class ValueTextBox : TextBox {
        public static readonly object valueChanged;

        public event EventHandler ValueChanged {
            add { Events.AddHandler(valueChanged, value); }
            remove { Events.RemoveHandler(valueChanged, value); }
        }

        protected virtual void RaiseValueChanged() {
            EventHandler handler = Events[valueChanged] as EventHandler;
            if (handler != null) handler(this, EventArgs.Empty);
        }

        protected override void OnLostFocus(EventArgs e) {
            base.OnLostFocus(e);
            RaiseValueChanged();
        }

        protected override void OnKeyDown(KeyEventArgs e) {
            base.OnKeyDown(e);
            if (e.KeyCode == Keys.Enter || e.KeyCode == Keys.Tab) {
                RaiseValueChanged();
            }
            else if (e.KeyCode == Keys.Escape)
                Text = txt;
        }

        string txt;
        protected override void OnGotFocus(EventArgs e) {
            base.OnGotFocus(e);
            txt = Text;
        }
    }
}
