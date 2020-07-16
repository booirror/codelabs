
EventDispatcher = {}

Event = {
  RawTouch = 1,
  Call = 2,
  KeyDown = 3,
  Pause = 4,
  Resume = 5, 
  Socket = 6, 
  Message = 7,
  Back = 8,
  Timeout =9,
  PanelRequest = 10,
  End = 11,
}

EventState = {
  inactvie = 1,
  active = 2,
  removeMarked = 3,
}
EventDispatcher.isDispatching = false
EventDispatcher.m_listener = {};
EventDispatcher.m_userKey = Event.End;
EventDispatcher.vector = {}

EventDispatcher.newUserEvent = function()
  EventDispatcher.m_userKey = EventDispatcher.m_userKey + 1
  return EventDispatcher.m_userKey
end

EventDispatcher.register = function(event, obj, func)
  if EventDispatcher.m_listener[event] == nil then
    EventDispatcher.m_listener[event] = {}
  end
  local mark = EventDispatcher.isDispatching and EventState.inactvie or EventState.active
  local tab = EventDispatcher.m_listener[event]
  tab[#tab+1] = {obj = obj, func = func, mark = mark}
  if EventDispatcher.isDispatching then
    EventDispatcher.vector[#EventDispatcher.vector + 1] = {tab, #tab}
  end
end

EventDispatcher.unregister = function(event, obj, func)
  if not EventDispatcher.m_listener[event] then
    return
  end
  local tab = EventDispatcher.m_listener[event]
  for k, v in pairs(tab) do
    if v.obj == obj and v.func == func then
      if EventDispatcher.isDispatching then
        tab[k].mark = EventState.removeMarked;
        EventDispatcher.vector[#EventDispatcher.vector + 1] = {tab, k}
      else
        tab[k] = nil;
      end
    end
  end
end

EventDispatcher.dispatch = function(event, ...)
  if not EventDispatcher.m_listener[event] then
    return
  end
  EventDispatcher.isDispatching = true
  local tab = EventDispatcher.m_listener[event]
  for k, v in pairs(tab) do
    if v.func and v.mark == EventState.active then
      v.func(v.obj, ...)
    end
  end
  EventDispatcher.isDispatching = false;
  
end

EventDispatcher.cleanup = function()
  for _, tab in pairs(EventDispatcher.vector) do
    if tab[1][tab[2]].mark == EventState.inactvie then
      tab[1][tab[2]].mark = EventState.active
    end
    if tab[1][tab[2]].mark == EventState.removeMarked then
      tab[1][tab[2]] = nil
    end
  end
  EventDispatcher.vector = {}
end






