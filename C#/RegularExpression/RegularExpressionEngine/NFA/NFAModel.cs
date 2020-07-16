using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace RegularExpressionEngine.NFA
{
    public class NFAModel
    {
        private ReadOnlyCollection<NFAState> _readonlyStates;
        private List<NFAState> _states;

        internal NFAModel()
        {
            _states = new List<NFAState>();
            _readonlyStates = new ReadOnlyCollection<NFAState>(_states);
        }

        public NFAState TailState { get; internal set; }

        public NFAEdge EntryEdge { get; internal set; }

        public int CaptureNum { get; internal set; }

        public ReadOnlyCollection<NFAState> States
        {
            get
            {
                return _readonlyStates;
            }
        }

        internal void AddState(NFAState state)
        {
            _states.Add(state);
            state.Index = _states.Count - 1;
        }

        internal void AddStates(IEnumerable<NFAState> states)
        {
            foreach (var s in states)
            {
                AddState(s);
            }
        }

    }
}
