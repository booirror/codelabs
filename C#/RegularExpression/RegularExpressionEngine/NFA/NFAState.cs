
using RegularExpressionEngine.Common;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Diagnostics;
namespace RegularExpressionEngine.NFA
{
    [DebuggerDisplay("State #{Index}")]
    public class NFAState
    {
        private List<NFAEdge> _outEdges;
        private ReadOnlyCollection<NFAEdge> _readonlyOutEdges;

        internal NFAState()
        {
            _outEdges = new List<NFAEdge>();
            _readonlyOutEdges = new ReadOnlyCollection<NFAEdge>(_outEdges);
            TokenIndex = -1;
        }

        public int Index {get; internal set;}

        internal int TokenIndex {get;set;}

        public ReadOnlyCollection<NFAEdge> OutEdges
        {
            get { return _readonlyOutEdges; }
        }

        internal void AddEmptyEdgeTo(NFAState targetState)
        {
            CodeContract.RequiresArgumentNotNull(targetState, "targetState");
            _outEdges.Add(new NFAEdge(targetState));
        }

        internal void AddEdge(NFAEdge edge)
        {
            CodeContract.RequiresArgumentNotNull(edge, "edge");
            _outEdges.Add(edge);
        }
    }
}
