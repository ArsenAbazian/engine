using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor.DefaultStyler
{
	public interface IStyler
	{
		void StartParse(DocumentRowCollection rows, int currentState);
		
		void GetStyles(int rowIndex);

		int CurrentState { get; }
	}
}
