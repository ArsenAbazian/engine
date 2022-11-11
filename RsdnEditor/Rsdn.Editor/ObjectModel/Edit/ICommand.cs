using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor.ObjectModel.Edit
{
	/// <summary>
	///  оманда редакрировни€ текста.
	/// </summary>
	public interface ICommand
	{
		/// <summary>
		/// ¬ыполн€ет работу по редактированию текста документа. ¬озвращает 
		/// обращенную команду (выполнение после которой данной может врнуть 
		/// текст документа в исходное состо€ние).
		/// </summary>
		/// <returns>
		/// ќбращенна€ команда (ее выполнение отмен€ет изменени€ внесенные
		/// данной командой, тем самым востанавлива€ текст документа).</returns>
		ICommand Execute();

		ICommand Execute(Range<Document> previousSelection);

		/// <summary>
		/// ƒокумент к которому примен€етс€ редактировние.
		/// </summary>
		Document Document { get; }

		/// <summary>
		/// –егион выделени€ до применени€ команды.
		/// </summary>
		Range<Document> SelectionBefor { get; }
		/// <summary>
		/// –егион выделени€ после применени€ команды.
		/// </summary>
		Range<Document> SelectionAfter { get; }
	}
}
