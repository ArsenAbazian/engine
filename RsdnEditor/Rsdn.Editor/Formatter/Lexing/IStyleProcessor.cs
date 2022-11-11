using System;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

namespace Rsdn.Editor.Lexing
{
	public interface IStyleProcessor
	{
		void ProcessStyle(XmlReader reader, int styleCode);
		
	}
}