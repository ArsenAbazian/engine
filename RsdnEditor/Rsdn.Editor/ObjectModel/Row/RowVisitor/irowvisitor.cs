using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor
{
	public interface IRowVisitor
	{
		void Visit(FoldedRow row);
		void Visit(HyphenationRow row);
		void Visit(DocumentRow row);
	}
}
