using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace Rsdn.Editor
{
	public struct Position<T> 
		: IComparable<Position<T>>, IEqualityComparer<Position<T>>
	{
		#region Конструкторы

		[DebuggerHidden]
		public Position(int line, int character)
		{
			_line = line;
			_character = character;
		}
		
		#endregion

		#region Свойство Line

		private int _line;

		public int Line
		{
			[DebuggerHidden]
			get { return _line; }
			[DebuggerHidden]
			set { _line = value; }
		}
		
		#endregion

		#region Свойство Character

		private int _character;

		public int Character
		{
			[DebuggerHidden]
			get { return _character; }
			[DebuggerHidden]
			set { _character = value; }
		}
		
		#endregion

		#region Max, Min

		public Position<T> Max(Position<T> second)
		{
			return this >= second ? this : second;
		}

		public Position<T> Min(Position<T> second)
		{
			return this < second ? this : second;
		}

		#endregion

		#region Арифметические операторы

		//public static PositionSpan<T> operator -(Position<T> x, Position<T> y)
		//{
		//  if (x.Line == y.Line)
		//  {
		//  }

		//  int line = x.Line;
		//  int ch = x.Character;

		//  return new PositionSpan<T>(x.Line - y.Line, x.Character - y.Character);
		//}

		//public static Position<T> operator +(Position<T> x, PositionSpan<T> y)
		//{
		//  return new PositionSpan<T>(x.Line + y.Line, x.Character + y.Character);
		//}

		#endregion

		#region Операторы сравнения

		public bool Check() { return _line >= 0 && _character >= 0; }

		[DebuggerHidden]
		public static bool operator ==(Position<T> x, Position<T> y)
		{
			return x._line == y._line && x._character == y._character;
		}

		[DebuggerHidden]
		public static bool operator !=(Position<T> x, Position<T> y)
		{
			return x._line != y._line || x._character != y._character;
		}

		[DebuggerHidden]
		public static bool operator >(Position<T> x, Position<T> y)
		{
			if (x._line > y._line)
				return true;

			if (x._line == y._line)
				return x._character > y._character;

			return false;
		}

		[DebuggerHidden]
		public static bool operator >=(Position<T> x, Position<T> y)
		{
			if (x._line > y._line)
				return true;

			if (x._line == y._line)
				return x._character >= y._character;

			return false;
		}

		[DebuggerHidden]
		public static bool operator <(Position<T> x, Position<T> y)
		{
			return y > x;
		}

		[DebuggerHidden]
		public static bool operator <=(Position<T> x, Position<T> y)
		{
			return y >= x;
		}

		#endregion

		#region Переопределение членов object-а

		[DebuggerHidden]
		public override bool Equals(object obj)
		{
			if (obj == null)
				return false;

			return this == (Position<T>)obj;
		}

		[DebuggerHidden]
		internal int GetHashCodeInternal()
		{
			return _line << 3 + _character;
		}

		[DebuggerHidden]
		public override int GetHashCode()
		{
			return GetHashCodeInternal();
		}

		[DebuggerHidden]
		public override string ToString()
		{
			return "Line " + Line + "; " + Character;
		}
		
		#endregion

		#region IComparable<Position> Members

		[DebuggerHidden]
		public int CompareTo(Position<T> other)
		{
			if (_line == other._line)
				return _character - other._character;

			return _line - other._line;
		}

		#endregion

		#region IEqualityComparer<Position> Members

		[DebuggerHidden]
		public bool Equals(Position<T> x, Position<T> y)
		{
			return x == y;
		}

		[DebuggerHidden]
		public int GetHashCode(Position<T> obj)
		{
			return obj.GetHashCodeInternal();
		}

		#endregion
	}
}
