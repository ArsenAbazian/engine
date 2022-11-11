using System;
using System.Collections.Generic;
using System.Text;

namespace Rsdn.Editor
{
	class GetRealRowVisitor : IRowVisitor
	{
		public DocumentRow GetRealRow(Row row)
		{
			row.AcceptVisitor(this);
			return _row;
		}

		void IRowVisitor.Visit(FoldedRow row)
		{
			throw new Exception("The method or operation is not implemented.");
		}

		void IRowVisitor.Visit(HyphenationRow row)
		{
			_row = row.RealDocumentRow;
		}

		void IRowVisitor.Visit(DocumentRow row)
		{
			_row = row;
		}

		private DocumentRow _row;
	}
}
