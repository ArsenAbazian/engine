using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	/// ѕозвол€ет сгрупировать несколько команд редактировани€ и представить
	/// их в виде одной единой команды.
	/// </summary>
	public class MultiCommand : Command, ICommand
	{
		#region  онструкторы

		public MultiCommand(Document document,
			Range<Document> selectionBefore, Range<Document> selectionAfter,
			params ICommand[] commands)
			: base(document, selectionBefore, selectionAfter, true)
		{
			_commands = commands;
		}

		public MultiCommand(Document document, params ICommand[] commands)
			: base(document, default(Range<Document>), default(Range<Document>), false)
		{
			_commands = commands;
		}
		
		#endregion

		#region Commands property

		[DebuggerBrowsable(DebuggerBrowsableState.Never)]
		ICommand[] _commands;

		public ICommand[] Commands
		{
			get { return _commands; }
		}

		#endregion

		#region ICommand Members

		public ICommand Execute(Range<Document> previousSelection)
		{
			IView activeView = Document.ActiveView;
			ICommand[] inversedCommands = new ICommand[Commands.Length];
			ICommand[] commands = Commands;

			foreach (IFreezeRepaint freezeRepaint in Document.InternalViews)
				freezeRepaint.FreezeRepaint();

			try
			{
				for (int i = 0, k = inversedCommands.Length - 1; i < commands.Length; i++)
					inversedCommands[k--] = commands[i].Execute();
			}
			finally
			{
				foreach (IFreezeRepaint freezeRepaint in Document.InternalViews)
					freezeRepaint.UnfreezeRepaint();
			}

			if (RestoreSelection && SelectionAfter != Range<Document>.Invalid)
				activeView.Selection.Range = SelectionAfter;

			return new MultiCommand(Document, 
				activeView.Selection.Range, previousSelection, inversedCommands);
		}

		public ICommand Execute()
		{
			return Execute(SelectionBefor);
		}

		#endregion

		#region ToString

		public override string ToString()
		{
			StringBuilder sb = new StringBuilder();

			sb.AppendLine("MultiCommand:");

			foreach (ICommand command in Commands)
				sb.AppendLine(command.ToString());

			return sb.ToString();
		}
 
		#endregion
	}
}
