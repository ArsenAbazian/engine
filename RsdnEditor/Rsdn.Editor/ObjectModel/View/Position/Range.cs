using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;

namespace Rsdn.Editor
{
	public struct Range<T>
	{
		#region Конструкторы

		public Range(Position<T> start, Position<T> end)
		{
			//Debug.Assert(start >= end);

			Start = start;
			End = end;
		}

		public Range(int startLine, int startCharacter,
			int endLine, int endCharacter)
		{
            if((startLine > endLine) || ((startLine == endLine) && (startCharacter > endCharacter))) {
                throw new ArgumentException("Range(int startLine, int startCharacter, int endLine, int endCharacter) " + startLine.ToString() + ", " + startCharacter.ToString() + ", " + endLine.ToString() + ", " + endCharacter.ToString());
            }

			Start = new Position<T>(startLine, startCharacter);
			End = new Position<T>(endLine, endCharacter);
		} 
		
		#endregion

		/// <summary>
		/// Недопустимое значение. Используется для инициализации заведомо
		/// недопустимым значением и для проверки допустимости значений.
		/// </summary>
		public static readonly Range<T> Invalid = new Range<T>(-1, -1, -1, -1);

		/// <summary>Начало диапазона.</summary>
		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		public Position<T> Start;
		/// <summary>Конец диапазона.</summary>
		[SuppressMessage("Microsoft.Design", "CA1051:DoNotDeclareVisibleInstanceFields")]
		public Position<T> End;

		public bool Check() { return Start.Check() && End.Check(); }

		#region Операторы сравнения

		[DebuggerHidden]
		public static bool operator ==(Range<T> x, Range<T> y)
		{
			return x.Start == y.Start && x.End == y.End;
		}

		[DebuggerHidden]
		public static bool operator !=(Range<T> x, Range<T> y)
		{
			return x.Start != y.Start || x.End != y.End;
		}

		public override bool Equals(object obj)
		{
			if (obj == null)
				return false;

			return ((Range<T>)obj) == this;
		}

		public override int GetHashCode()
		{
			return Start.GetHashCode() + (End.GetHashCode() << 6);
		}

		public bool IsEmpty()
		{
			return Start == End;
		}

		#endregion


		#region Свойство StartLine

		public int StartLine
		{
			get { return Start.Line; }
			set { Start.Line = value; }
		}
		
		#endregion

		#region Свойство EndLine

		public int EndLine
		{
			get { return End.Line; }
			set { End.Line = value; }
		}
		
		#endregion

		#region Свойство StartLineCharacter

		public int StartLineCharacter
		{
			get { return Start.Character; }
			set { Start.Character = value; }
		}

		#endregion

		#region Свойство EndLineCharacter

		public int EndLineCharacter
		{
			get { return End.Character; }
			set { End.Character = value; }
		}

		#endregion

		public override string ToString()
		{
			return "Start: " + Start + "; End: " + End + "; ";
		}
	}
}
