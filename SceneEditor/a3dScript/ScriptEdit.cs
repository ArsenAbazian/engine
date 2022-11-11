using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using Intillisense;
using Rsdn.Editor;
using Rsdn.Editor.a3dScript;
using System.ComponentModel;
using System.Windows.Forms;
using classMapper;
using Rsdn.Editor.ObjectModel;
using Rsdn.Editor.ObjectModel.Edit;

namespace sceneEditor.a3dScript {
    public class ScriptEditWithIntellisence : ScriptEdit {
        private System.Windows.Forms.ImageList imageList1;

        public ScriptEditWithIntellisence() {
            InitializeComponent();
        }

        GListBox listBox;
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        public GListBox PopupListBox { 
            get {
                if (listBox == null) listBox = CreateListBox();
                return listBox; 
            } 
        }

        protected virtual GListBox CreateListBox() {
            GListBox lb = new GListBox();
            lb.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            lb.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawVariable;
            lb.ImageList = this.imageList1;
            lb.Name = "listBoxAutoComplete";
            lb.Size = new Size(150, 200);
            lb.MaximumSize = new Size(150, 200);
            lb.TabIndex = 3;
            lb.Visible = false;
            //lb.KeyDown += new System.Windows.Forms.KeyEventHandler(this.listBoxAutoComplete_KeyDown);
            //lb.DoubleClick += new System.EventHandler(this.listBoxAutoComplete_DoubleClick);
            //lb.SelectedIndexChanged += new System.EventHandler(this.listBoxAutoComplete_SelectedIndexChanged);
            Controls.Add(lb);
            return lb;
        }

        public virtual void ShowListBox(Point location) {
            PopupListBox.Location = location;
            PopupListBox.BringToFront();
            PopupListBox.Show();
            PopupListBox.SelectedIndex = 0;
        }

        public override bool ShouldChangeLine(bool isExtendSelection, int line) {
            if(PopupListBox.Visible) {
                PopupListBox.SelectedIndex = Math.Min(PopupListBox.Items.Count - 1, Math.Max(0, PopupListBox.SelectedIndex + line));
                return false;
            }
            return base.ShouldChangeLine(isExtendSelection, line);
        }

        protected virtual bool IsValidWord(Range<Document> range) {
            return range != InvalidRange;
        }

        protected virtual void ProcessTab() {
            Range<Document> word = GetReplaceWord(ActiveView.Selection.End);
            if(!IsValidWord(word)) return;
            string text = ((GListBoxItem)PopupListBox.SelectedItem).Text;
            UpdateCommand comm = new UpdateCommand(Document, text, word.Start, word.End);
            comm.Execute();
            PopupListBox.Visible = false;
        }
        
        public override void ProcessViewKeyPress(KeyPressEventArgs e) {
            if(PopupListBox.Visible) {
                if(e.KeyChar == '\t') {
                    ProcessTab();
                    e.Handled = true;
                    return;
                }
            }
            base.ProcessViewKeyPress(e);
        }

        protected virtual void ProcessPopupListBoxNavigation(KeyEventArgs e) {
            if(e.KeyCode == Keys.Up && PopupListBox.SelectedIndex > 0) {
                PopupListBox.SelectedIndex--;
                e.Handled = true;
            }
            else if(e.KeyCode == Keys.Down && PopupListBox.SelectedIndex < PopupListBox.Items.Count - 1) {
                PopupListBox.SelectedIndex++;
                e.Handled = true;
            }
        }

        protected virtual bool ShouldShowPopupListBox(Rsdn.Editor.SelectionChangedEventArgs e) {
            if(e.View.Document.Rows.Count <= e.EndPosition.Line) return false;
            if(e.EndPosition.Character == 0) return false;
            return e.View.Document.Rows[e.EndPosition.Line].Text[e.EndPosition.Character - 1] == '.';
        }

        protected virtual Point GetListBoxPoint(Rectangle selectionRect) {
            Point res = new Point(selectionRect.X - 22, selectionRect.Bottom);
            int deltaUp = selectionRect.Top - PopupListBox.Height;
            int deltaDown = Height - (selectionRect.Bottom + PopupListBox.Height);

            if(res.X < 0) res.X = 0;
            if(res.X + PopupListBox.Width > Width - 26)
                res.X = Width - 26 - PopupListBox.Width;

            if(deltaDown < 0) {
                if(deltaUp > deltaDown) {
                    if(deltaUp > 0)
                        res.Y = selectionRect.Top - PopupListBox.Height;
                    else {
                        res.Y = 0;
                        PopupListBox.Height = selectionRect.Top;
                    }
                }
                else {
                    res.Y = selectionRect.Bottom;
                    PopupListBox.Height = Height - selectionRect.Bottom;
                }
            }
            else {
                res.Y = selectionRect.Bottom;
            }
            return res;
        }

        public static Range<Document> InvalidRange = new Range<Document>(-1, -1, -1, -1);

        protected virtual Range<Document> GetReplaceWord(Position<Document> pos) {
            DocumentRow currRow = Document.Rows[pos.Line];
            int i;
            for(i = pos.Character - 1; i >= 0; i--) {
                if(Utils.IsPunctuation(currRow.Text[i])) break;
            }
            i = i < 0 ? 0 : i + 1;
            return new Range<Document>(pos.Line, i, pos.Line, pos.Character);
        }

        protected virtual Range<Document> GetPrevWord(Position<Document> pos) {
            if(pos.Character < 0) return InvalidRange;
            DocumentRow currRow = null;
            if(pos.Line >= Document.Rows.Count) return InvalidRange;
            currRow = Document.Rows[pos.Line];
            int i;
            for(i = pos.Character - 1; i >= 0; i--) {
                if(Utils.IsPunctuation(currRow.Text[i])) break;
            }
            i = i < 0 ? 0 : i+1;
            if(i == pos.Character) return InvalidRange;
            return new Range<Document>(pos.Line, i, pos.Line, pos.Character);
        }

        protected virtual Range<Document> GetFirstWord(Position<Document> pos) {
            Range<Document> range = InvalidRange, tmp = new Range<Document>(pos, pos);
            while(true) {
                tmp = GetPrevWord(tmp.Start);
                if(tmp == InvalidRange) break;
                range = tmp;
            }
            return range;
        }

        IDependencyTreeNode root;
        public IDependencyTreeNode Root { 
            get { return root; }
            set { root = value; }
        }

        protected virtual void PopulateListBoxByDependencyProperty(SelectionChangedEventArgs e) {
            PopupListBox.Items.Clear();

            Position<Document> pos = e.EndPosition;
            pos.Character--;
            bool firstTime = true;
            Range<Document> word = InvalidRange;
            IDependencyTreeNode currentNode = null;
            while(true) {
                word = firstTime ? GetFirstWord(pos) : GetNextWord(e.View, word);
                firstTime = false;
                if(word == InvalidRange) break;
                currentNode = GetChildNode(currentNode, e.View.Document.Rows[word.StartLine].Text.Substring(word.StartLineCharacter, word.EndLineCharacter - word.StartLineCharacter));
            }
            PopuplateListBox(currentNode);
        }

        protected void PopuplateListBox(IDependencyTreeNode currentNode) {
            if(currentNode == null) return;
            foreach(IDependencyTreeNode node in currentNode.Children) {
                PopupListBox.Items.Add(new GListBoxItem(node.NodeName, 0));
            }
        }

        protected virtual Range<Document> GetNextWord(IView view, Range<Document> word) {
            int i;
            if(word.EndLineCharacter + 1 >= view.Document.Rows[word.StartLine].Text.Length) return InvalidRange;
            for(i = word.EndLineCharacter + 1; i < view.Document.Rows[word.StartLine].Text.Length; i++) {
                if(Utils.IsPunctuation(view.Document.Rows[word.StartLine].Text[i])) break;
            }
            return new Range<Document>(word.StartLine, word.EndLineCharacter + 1, word.StartLine, i);
        }

        protected IDependencyTreeNode GetChildNode(IDependencyTreeNode currentNode, string name) {
            if(currentNode == null && name == Root.NodeName) return Root;
            foreach(IDependencyTreeNode node in currentNode.Children) {
                if(node.NodeName.StartsWith(name)) return node;
            }
            return null;
        }

        protected virtual void PopulateListBox(SelectionChangedEventArgs e) {
            PopulateListBoxByDependencyProperty(e);
        }
        
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ScriptEditWithIntellisence));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "Properties.png");
            // 
            // ScriptEdit
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.Name = "ScriptEdit";
            this.SelectionChanged += new System.EventHandler<Rsdn.Editor.SelectionChangedEventArgs>(this.ScriptEdit_SelectionChanged);
            this.ResumeLayout(false);

        }

        private System.ComponentModel.IContainer components;

        private void ScriptEdit_SelectionChanged(object sender, SelectionChangedEventArgs e) {
            if(ShouldShowPopupListBox(e)) {
                PopulateListBox(e);
                Position<IView> viewPos = ActiveView.ToView(Document.ActiveView.Selection.Start);
                Rectangle rect = (Rectangle)ActiveView.ToLocation(viewPos);
                ShowListBox(GetListBoxPoint(rect));
            }
        }
    }
}
