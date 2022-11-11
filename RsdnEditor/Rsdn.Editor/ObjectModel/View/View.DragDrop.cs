// DragDrop
using System;
using System.Drawing;
using System.Windows.Forms;
using System.Diagnostics;
using Rsdn.Editor.ObjectModel;
using Rsdn.Editor.ObjectModel.Edit;

namespace Rsdn.Editor
{
	partial class View
	{
		#region Описание последовательностей вызовов событий.

		// Последовательность вызовов событий при перетаскивании 
		// из редатора:
		//   QueryContinueDrag
		//   DragEnter
		//   GiveFeedback
		//   DragOver
		//   GiveFeedback
		//   QueryContinueDrag
		//   ...
		//   DragOver
		//  GiveFeedback

		// Последовательность действий при перетаскивании в редактр:
		//   DragEnter
		//   DragOver
		//   ...
		//   DragDrop

		// Последовательность действий при перетаскивании внутри одного
		// и того же представления:
		//   QueryContinueDrag
		//   DragEnter
		//   GiveFeedback
		//   DragOver
		//   GiveFeedback
		//   QueryContinueDrag
		//   ...
		//   DragDrop
		
		#endregion

		#region Переменные и константы.

		/// <summary>
		/// Размер области по бокам представления рассматриваемых как область
		/// начала прокрутки. При удержании в этой области курсора через 
		/// определенный промежуток времени начинается автоматическая 
		/// пророкрутка.
		/// </summary>
		const int ScrollAreaSize = 5;

		/// <summary>
		/// Тип DragDropEffect-а который был в момент перетаскивания.
		/// </summary>
		DragDropEffects _lastDragDropEffects;

		/// <summary>
		/// Если true - происходит перемешение текста в пределах 
		/// одного представления.
		/// Иначе перетаскивание
		/// </summary>
		private bool _dragOverThisView;
		
		/// <summary>
		/// Происходит перемещение текста (текст должен быть удален после
		/// копирования его на новое место).
		/// </summary>
		private bool _isMove;

		/// <summary>
		/// Позия в которой находился курсор когда последний раз 
		/// было получено сообщение DragOver.
		/// </summary>
		private Rectangle _oldDragPosition = new Rectangle(-1, -1, -1, -1);

		/// <summary>
		/// Значение используемое по умолчанию для переменной _oldDragPosition.
		/// Это значение не может являться корректной координатой, стало быть
		/// если _oldDragPosition получает это значение, то в этом цикле DragDrop
		/// до этого еще не было событий DragOver.
		/// </summary>
		private readonly Rectangle DefaultDragPosition =
			new Rectangle(-1, -1, -1, -1);

		#endregion

		#region BeginDrag

		/// <summary>
		/// Начинает операцию перетаскивания.
		/// </summary>
		/// <param name="mouseLocation">Позиция мыши.</param>
		private void BeginDrag(Point mouseLocation)
		{
			Debug.WriteLineIf(TraceDragDrop.TraceVerbose, "BeginDrag: mouseLocation = " + mouseLocation);
			
			string text = Document.GetText(_selectionStartDocument, 
				_selectionEndDocument);

			_isMove = false;
			DoDragDrop(new DataObject(text), DragDropEffects.All);
		}
		
		#endregion

		#region GiveFeedback

		/// <summary>
		/// Вызывается в мемент когда редактор находится в режиме 
		/// перетаскивания (т.е. когда текст из редактора перетаскивается
		/// кудато).
		/// Исользуется для того чтобы запомнить какой DragDropEffects 
		/// был в этом момент.
		/// </summary>
		private void View_GiveFeedback(object sender, GiveFeedbackEventArgs e)
		{
			_lastDragDropEffects = e.Effect;
			Debug.WriteLineIf(TraceDragDrop.TraceVerbose, "GiveFeedback " + sender + " e = " + ToStr(e));
		}
		
		#endregion

		#region QueryContinueDrag

		/// <summary>
		/// Вызывается при перетаскивании текста из редатора.
		/// В этом событии можно прервать перетаскивание. В нем же
		/// закачивается процедура перетаскивания (когда в e.Action 
		/// передается значение DragAction.Drop) если перетаскивание
		/// производится во внешний редактор.
		/// </summary>
		private void View_QueryContinueDrag(object sender, 
			QueryContinueDragEventArgs e)
		{
			// Отменяем перетаскивание если ажета Esc.
			if (e.EscapePressed)
			{
				e.Action = DragAction.Cancel;
				DrawOldDragPosition();
				ResetOldDragPosition();
				return;
			}

			if (e.Action == DragAction.Drop
				&& _lastDragDropEffects == DragDropEffects.Move)
			{
				// Если перетаскиваниее производится в рамках одного представления
				// запоминаем в _isMove, что нужно произвести перемещение.
				if (_dragOverThisView)
					_isMove = true;
				else
					Delete();
			}

			Debug.WriteLineIf(TraceDragDrop.TraceVerbose, "QueryContinueDrag " + sender + " e = " + ToStr(e));
		}
		
		#endregion

		#region DragDrop

		/// <summary>
		/// Окончание перетаскивания пореведшее к вставке текста в 
		/// данное представление.
		/// </summary>
		private void View_DragDrop(object sender, DragEventArgs e)
		{
			#region Извлечение текста из буфера обмена в переменную text.

			// Проверяем лежит ли в клипборде поддерживаемый редактором формат...
			string text = GetTextFromDragEventArgs(e);

			// Если text равен пустой строке, значит поддерживаемый формат
			// не найден и нужно завазывать возиться с этим недоразумением.
			if (text == null)
				return;

			#endregion	

			#region Инициализация переменных.

			// Переводим координаты вставки в позицию в документе.
			Position<Document> docInsPos = ToDocument(
				PointToClient(new Point(e.X, e.Y)));

			// Верхняя (минимальная) позиция в документе.
			Position<Document> selectionBottom = _selectionStartDocument.Max(
				_selectionEndDocument);
			// Нижняя (максимальная) позиция в документе.
			Position<Document> selectionTop = _selectionStartDocument.Min(
				_selectionEndDocument);
			Position<Document> start = _selectionStartDocument;
			Position<Document> end = _selectionEndDocument;
			DeleteCommand delCmd = null;
			
			#endregion

			// Если текст передвигается в рамках одного документа...
			//TODO: пока что в рамках одного представления. Нужно поправить.
			if (_isMove)
			{
				// Формируем команду удаления.
				delCmd = new DeleteCommand(Document, start, end);

				#region Описние корректировки позиции вставки.

				// Если нужно кооректируем позцию вствки
				// Тут могут быть варианты.
				// 1. Вставка производится перед выделением.
				//    При этом кооректировка позиции вставки не требуется.
				// 2. Вставка производится в строку находящуюся ниже строки выделения.
				//    Нужно откорректировать строку вставки, но не трогать символ.
				// 3. Вставка производится в конец последней строки выделения.
				//    Нужно откорректировать как строку вставки, так и символ.
				// 4. Производится попытка вставить текст внутрь выделения.
				//    Нужно прекратить операцию не внося никаких изменений.
				
				#endregion

				#region Проверка попадания дропа в выделение. Случай 4.

				if (Utils.Between(selectionTop, selectionBottom, docInsPos))
				{
					// Если в процессе данного цикла перетаскивания у представления
					// вызывался метод DragOver, то нужно стереть изображение позиции
					// вставки (которая была рарисована в DragOver).
					DrawOldDragPosition();
					ResetOldDragPosition();
					return;
				}
				
				#endregion

				#region Корретировка позиции вставки. Случай 2 или 3.

				if (docInsPos.Line >= selectionBottom.Line) // Случай 2 или 3.
				{
					if (docInsPos.Line == selectionBottom.Line
						&& docInsPos.Character > selectionBottom.Character) // Случай 3.
					{
						// При вставке в последнюю строку символ вставки нужно 
						// откорректировать так чтобы он был равен верхней
						// позиции выделения плюс количество символов между
						// концом выделения и позицией вставки.
						docInsPos.Character =
							selectionTop.Character
							// длинна отрезка с началом в конце выделения и до
							// позиции вставки.
							+ (docInsPos.Character - selectionBottom.Character);
					}

					// При вставке за выделением нужно вычесть количество выделенных
					// строк из позиции вставки.
					docInsPos.Line -= selectionBottom.Line - selectionTop.Line;
				}
				
				#endregion
			}

			ResetOldDragPosition();

			// Формируем команду вставки.
			InsertCommand insCmd = new InsertCommand(
				Document, text, docInsPos, true);

			// Выполняем команду редактирования.
			if (delCmd == null)
				Document.ExecuteCommand(insCmd);
			else
				// Для того чтобы удаление текста и его вставка воспринимались
				// редактором как одна "атомарная операция перемещения текста"
				// нужно, перед их выполнением, объеденить эти команды с 
				// помощью MultiCommand.
				Document.ExecuteCommand(new MultiCommand(Document, delCmd, insCmd));

			Debug.WriteLineIf(TraceDragDrop.TraceVerbose, "DragDrop " + sender + " e = " + ToStr(e));
		}

		#endregion

		#region DragEnter

		/// <summary>
		/// Курсор в режиме перетаскивания попал в данное представление.
		/// </summary>
		private void View_DragEnter(object sender, DragEventArgs e)
		{
			_dragOverThisView = true;
			Debug.WriteLineIf(TraceDragDrop.TraceVerbose, "DragEnter " + sender + " e = " + ToStr(e));
		}
		
		#endregion

		#region DragLeave

		/// <summary>
		/// Курсор в режиме перетаскивания ушел из данного представления.
		/// </summary>
		private void View_DragLeave(object sender, EventArgs e)
		{
			_dragOverThisView = false;
			Debug.WriteLineIf(TraceDragDrop.TraceVerbose, "DragLeave " + sender + " e = " + ToStr(e));
		}
		
		#endregion

		#region DragOver

		/// <summary>
		/// Курсор в режиме перетаскивания передвигается или находится
		/// над данным представлением.
		/// </summary>
		private void View_DragOver(object sender, DragEventArgs e)
		{
			if (sender == this)
			{
				if (Control.ModifierKeys == Keys.Control)
					e.Effect = DragDropEffects.Copy;
				else
					e.Effect = DragDropEffects.Move;
			}
			else
			{
				if (Control.ModifierKeys == Keys.Shift)
				{
					if ((e.AllowedEffect & DragDropEffects.Move) != 0)
						e.Effect = DragDropEffects.Move;
				}
				else
					if ((e.AllowedEffect & DragDropEffects.Copy) != 0)
						e.Effect = DragDropEffects.Copy;
			}

			DrawOldDragPosition();

			Point pt = PointToClient(new Point(e.X, e.Y));
			Rectangle textRect = TextAreaRectangle;

			if (textRect.Bottom - pt.Y <= ScrollAreaSize)
				ScrollLineDowd();
			else if (pt.Y - textRect.Top <= ScrollAreaSize)
				ScrollLineUp();

			if (textRect.Right - pt.X <= ScrollAreaSize)
				ScrollColRight();
			else if (pt.X - textRect.Left <= ScrollAreaSize)
				ScrollColLeft();

			HitTestInfo info = HitTest(pt, true);

			if (info.ViewArea == ViewArea.Text)
			{
				Rectangle? rect = ToLocation(info.Position);
				if (rect.HasValue)
				{
					_oldDragPosition = RectangleToScreen(rect.Value);
					_oldDragPosition.Width = 2;

					DrawOldDragPosition();
				}
			}

			Debug.WriteLineIf(TraceDragDrop.TraceVerbose, "DragOver " + sender + " e = " + ToStr(e));
		}
 
		#endregion

		#region Вспомогательные функции.

		/// <summary>
		/// Сбрасывает позию (_oldDragPosition) в которой находился курсор 
		/// когда последний раз было получено сообщение DragOver в значение 
		/// по умолчанию. Если _oldDragPosition содержит это значе, то 
		/// отрисовка позиции вставки (Drop-а) не производится.
		/// </summary>
		private void ResetOldDragPosition()
		{
			_oldDragPosition = DefaultDragPosition;
		}

		/// <summary>
		/// Если позия в которой находился курсор когда последний раз 
		/// было получено сообщение DragOver не равно DefaultDragPosition,
		/// инвертирует эту позицию тем самым стирая прошлый маркер 
		/// вставки.
		/// </summary>
		private void DrawOldDragPosition()
		{
			if (_oldDragPosition != DefaultDragPosition)
				ControlPaint.FillReversibleRectangle(_oldDragPosition, Color.Black);
		}

		/// <summary>
		/// Пытается извлечь текст из буфера обмена. Если функции этого
		/// не удается, то возвращается null.
		/// </summary>
		/// <param name="e">Аргумент события DragDrop.</param>
		/// <returns>Строка из буфера обмена или null.</returns>
		private static string GetTextFromDragEventArgs(DragEventArgs e)
		{
			string text = null;
			string[] formats = e.Data.GetFormats();

			if (Utils.Contain(formats, "System.String"))
				text = (string)e.Data.GetData("System.String");
			else if (Utils.Contain(formats, DataFormats.UnicodeText))
				text = (string)e.Data.GetData(DataFormats.UnicodeText);
			else if (Utils.Contain(formats, DataFormats.Text))
				text = (string)e.Data.GetData(DataFormats.Text);

			return text;
		}

		#endregion
	}
}
