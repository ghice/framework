package samchon.library.container
{
	/**
	 * Interface for Dictionary container having key-based items<br/>
	 * <br/> 
	 * @author Jeongho Nam<br/>
	 */ 
	//template <typename _Ty>
	public interface IDictionary
	{
		//function get(key:*):_Ty;
		
		/**
		 * Indicates whether a container has an object having the specified identifier.</br>
		 * </br>
		 * @param key The identifier wants to check
		 * @return If there's the object then true, otherwise false
		 */
		function has(key:*):Boolean;
		
		/**
		 * Removes child element by specified key and returns whether deletion was succeded or not.</br>
		 * </br>
		 * @param key the identifier of the Entity wants to erase 
		 * @return Whethere succeded to erase or not
		 */
		function erase(key:*):Boolean;
	}
}